#include <cplus/array.h>
#include <cplus/lexer.h>
#include <cplus/token.h>

#include <std/error/assert.h>
#include <std/memory/garbage_collector.h>

#include "cplus/macros.h"
#include "lexer_helper.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

__cplus__used static void lexer_ctor(void *instance, va_list *args);
__cplus__used static void lexer_dtor(void *instance);

#define peek(L) l_peek(L)
#define peek_n(L, n) l_peek_n(L, n)
#define next(L) l_next(L)
#define skip_whitespace(L) l_skip_whitespace_and_comments(L)

#define is_ident_start(c) isalnum((unsigned char) (c)) || (c) == '_'
#define is_ident_continue(c) isalnum((unsigned char) (c)) || (c) == '_'

#define __LEXER_METHOD                                                                                                 \
    const size_t start = L->_priv.pos;                                                                                 \
    const size_t line = L->_priv.line;                                                                                 \
    const size_t col = L->_priv.col;

#define __LEXER_NEW_BUFF                                                                                               \
    const size_t end = L->_priv.pos;                                                                                   \
    const size_t n = end - start;                                                                                      \
    char *buff;                                                                                                        \
    allocate(buff, n + 1);                                                                                             \
    memcpy(buff, L->_priv.io.buff + start, n);                                                                         \
    buff[n] = '\0';

static __cplus__const __inline const TokenKind _lookup_keyword(const char *s)
{
    for (unsigned int i = 0; KEYWORDS[i].kw != NULL; ++i) {
        if (strcmp(KEYWORDS[i].kw, s) == 0) {
            return KEYWORDS[i].k;
        }
    }
    return TOK_IDENT;
}

// clang-format off
static const Class __cplus__used lexer_class = {
    .__size__ = sizeof(Lexer),
    .__name__ = "Lexer",
    .__ctor__ = lexer_ctor,
    .__dtor__ = lexer_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used __inline const Class *LexerGetClass(void)
{
    return &lexer_class;
}

static __cplus__const __inline const Array *lexer_get_tokens(const Lexer *self)
{
    return self->_priv.tokens;
}

/**
 * lexer start
 */

static __inline void _lexer_file_exists(IOFile *out_io)
{
    __assert(stat(out_io->src, &out_io->st) == 0, "file does not exist");
    __assert(S_ISREG(out_io->st.st_mode) || S_ISLNK(out_io->st.st_mode), "file is not a regular file or symlink");
}

static __inline void _lexer_file_read(IOFile *out_io)
{
    out_io->stream = fopen(out_io->src, "r");
    allocate(out_io->buff, out_io->st.st_size + 1);

    const size_t br = fread(out_io->buff, sizeof(char), (size_t) out_io->st.st_size, out_io->stream);

    __assert(br == out_io->st.st_size && !feof(out_io->stream), "failed to read file");
    fclose(out_io->stream);
    out_io->buff[out_io->st.st_size] = '\0';
}

/**
 * tokensization
 */

static __cplus__pure Token *_lexer_identifier(Lexer *L)
{
    __LEXER_METHOD;

    next(L);
    while (is_ident_continue(peek(L))) {
        next(L);
    }

    /** TODO: no allocations bc buf is already allocated */
    __LEXER_NEW_BUFF;

    const TokenKind k = _lookup_keyword(buff);
    Token *t = _new_token(k, buff, line, col);

    return t;
}

static __cplus__pure Token *_lexer_number(Lexer *L)
{
    __LEXER_METHOD;
    bool is_float = false;

    while (isdigit((unsigned char) peek(L))) {
        next(L);
    }

    if (peek(L) == '.' && isdigit((unsigned char) peek_n(L, 1))) {
        is_float = true;
        next(L);
        while (isdigit((unsigned char) peek(L))) {
            next(L);
        }
    }

    const char p = peek(L);

    if ((p == 'e' || p == 'E')
        && (isdigit((unsigned char) peek_n(L, 1)) || peek_n(L, 1) == '+' || peek_n(L, 1) == '-')) {
        is_float = true;
        next(L);
        if (peek(L) == '+' || peek(L) == '-') {
            next(L);
        }
        while (isdigit((unsigned char) peek(L))) {
            next(L);
        }
    }

    /** TODO: no allocations bc buf is already allocated */
    __LEXER_NEW_BUFF;

    Token *t = _new_token(is_float ? TOK_FLOAT : TOK_INTEGER, buff, line, col);
    return t;
}

static __cplus__pure Token *_lexer_string(Lexer *L)
{
    __LEXER_METHOD;

    next(L);

    size_t cap = 64;
    size_t len = 0;
    char *buff;

    allocate(buff, cap);

    forever
    {
        const char c = peek(L);
        if (c == '\0') {
            buff[len] = '\0';
            Token *t = _new_token(TOK_ERROR, buff, line, col);
            return t;
        }

        if (c == '"') {
            next(L);
            buff[len] = '\0';
            Token *t = _new_token(TOK_STRING, buff, line, col);
            return t;
        }

        if (c == '\\') {
            next(L);
            const char esc = peek(L);
            char out = esc;

            switch (esc) {
                case 'n':
                    out = '\n';
                    break;
                case 'r':
                    out = '\r';
                    break;
                case 't':
                    out = '\t';
                    break;
                case '\\':
                    out = '\\';
                    break;
                case '"':
                    out = '"';
                    break;
                default:
                    break;
            }
            next(L);
            if (len + 1 >= cap) {
                cap *= 2;
                reallocate(buff, cap);
            }
            buff[len++] = out;

        } else {

            next(L);

            if (len + 1 >= cap) {
                cap *= 2;
                reallocate(buff, cap);
            }
            buff[len++] = c;
        }
    }
}

static Token *_lexer_next(Lexer *L)
{
    skip_whitespace(L);

    const size_t line = L->_priv.line;
    const size_t col = L->_priv.col;
    const char c = peek(L);

    if (c == '\0') {
        return _new_token(TOK_EOF, NULL, line, col);
    }

    if (is_ident_start(c)) {
        return _lexer_identifier(L);
    }

    if (isdigit((unsigned char) c)) {
        return _lexer_number(L);
    }

    if (c == '"') {
        return _lexer_string(L);
    }

    const char n1 = peek_n(L, 1);

    if (c == '-' && n1 == '>') {
        next(L);
        next(L);
        return _new_token(TOK_ARROW, "->", line, col);
    }

    if (c == '+' && n1 == '+') {
        next(L);
        next(L);
        return _new_token(TOK_PLUSPLUS, "++", line, col);
    }

    if (c == '-' && n1 == '-') {
        next(L);
        next(L);
        return _new_token(TOK_MINUSMINUS, "--", line, col);
    }

    if (c == '=' && n1 == '=') {
        next(L);
        next(L);
        return _new_token(TOK_EQ, "==", line, col);
    }

    if (c == '!' && n1 == '=') {
        next(L);
        next(L);
        return _new_token(TOK_NEQ, "!=", line, col);
    }

    if (c == '<' && n1 == '=') {
        next(L);
        next(L);
        return _new_token(TOK_LE, "<=", line, col);
    }

    if (c == '>' && n1 == '=') {
        next(L);
        next(L);
        return _new_token(TOK_GE, ">=", line, col);
    }

    if (c == '&' && n1 == '&') {
        next(L);
        next(L);
        return _new_token(TOK_AND, "&&", line, col);
    }

    if (c == '|' && n1 == '|') {
        next(L);
        next(L);
        return _new_token(TOK_OR, "||", line, col);
    }

    switch (c) {
        case '{':
            next(L);
            return _new_token(TOK_LBRACE, "{", line, col);
        case '}':
            next(L);
            return _new_token(TOK_RBRACE, "}", line, col);
        case '(':
            next(L);
            return _new_token(TOK_LPAREN, "(", line, col);
        case ')':
            next(L);
            return _new_token(TOK_RPAREN, ")", line, col);
        case '[':
            next(L);
            return _new_token(TOK_LBRACK, "[", line, col);
        case ']':
            next(L);
            return _new_token(TOK_RBRACK, "]", line, col);
        case ';':
            next(L);
            return _new_token(TOK_SEMI, ";", line, col);
        case ',':
            next(L);
            return _new_token(TOK_COMMA, ",", line, col);
        case ':':
            next(L);
            return _new_token(TOK_COLON, ":", line, col);
        case '.':
            next(L);
            return _new_token(TOK_DOT, ".", line, col);
        case '@':
            next(L);
            return _new_token(TOK_AT, "@", line, col);
        case '+':
            next(L);
            return _new_token(TOK_PLUS, "+", line, col);
        case '-':
            next(L);
            return _new_token(TOK_MINUS, "-", line, col);
        case '*':
            next(L);
            return _new_token(TOK_STAR, "*", line, col);
        case '/':
            next(L);
            return _new_token(TOK_SLASH, "/", line, col);
        case '%':
            next(L);
            return _new_token(TOK_PERCENT, "%", line, col);
        case '<':
            next(L);
            return _new_token(TOK_LT, "<", line, col);
        case '>':
            next(L);
            return _new_token(TOK_GT, ">", line, col);
        case '!':
            next(L);
            return _new_token(TOK_NOT, "!", line, col);
        case '=':
            next(L);
            return _new_token(TOK_ASSIGN, "=", line, col);
        default:
            next(L);
            char tmp[64];
            snprintf(tmp, sizeof(tmp), "unexpected character '%c'", c);
            return _new_token(TOK_ERROR, tmp, line, col);
    }
}

/**
 * entr point
 */

static void lexer_start(Lexer *self)
{
    struct _LexerData *priv = &self->_priv;

    _lexer_file_exists(&priv->io);
    _lexer_file_read(&priv->io);
    priv->len = priv->io.st.st_size;
    forever
    {
        Token *t = _lexer_next(self);

        if (!t || t->kind == TOK_EOF) {
            break;
        }
        priv->tokens->append(priv->tokens, t);
    }
}

/**
 * show method for debug purposes
 */

__cplus__const static const char *_to_string(TokenKind k)
{
    switch (k) {
        case TOK_EOF:
            return "EOF";
        case TOK_ERROR:
            return "ERROR";
        case TOK_IDENT:
            return "IDENT";
        case TOK_INTEGER:
            return "INTEGER";
        case TOK_FLOAT:
            return "FLOAT";
        case TOK_STRING:
            return "STRING";
        case TOK_DEF:
            return "DEF";
        case TOK_MODULE:
            return "MODULE";
        case TOK_STRUCT:
            return "STRUCT";
        case TOK_FOR:
            return "FOR";
        case TOK_IN:
            return "IN";
        case TOK_FOREVER:
            return "FOREVER";
        case TOK_BREAK:
            return "BREAK";
        case TOK_IF:
            return "IF";
        case TOK_ELSIF:
            return "ELSIF";
        case TOK_ELSE:
            return "ELSE";
        case TOK_RETURN:
            return "RETURN";
        case TOK_CONST:
            return "CONST";
        case TOK_DEFER:
            return "DEFER";
        case TOK_TRUE:
            return "TRUE";
        case TOK_FALSE:
            return "FALSE";
        case TOK_NULL:
            return "NULL";
        case TOK_LBRACE:
            return "{";
        case TOK_RBRACE:
            return "}";
        case TOK_LPAREN:
            return "(";
        case TOK_RPAREN:
            return ")";
        case TOK_LBRACK:
            return "[";
        case TOK_RBRACK:
            return "]";
        case TOK_SEMI:
            return ";";
        case TOK_COMMA:
            return ",";
        case TOK_COLON:
            return ":";
        case TOK_DOT:
            return ".";
        case TOK_AT:
            return "@";
        case TOK_PLUS:
            return "+";
        case TOK_MINUS:
            return "-";
        case TOK_STAR:
            return "*";
        case TOK_SLASH:
            return "/";
        case TOK_PERCENT:
            return "%";
        case TOK_PLUSPLUS:
            return "++";
        case TOK_MINUSMINUS:
            return "--";
        case TOK_EQ:
            return "==";
        case TOK_ASSIGN:
            return "=";
        case TOK_NEQ:
            return "!=";
        case TOK_LT:
            return "<";
        case TOK_GT:
            return ">";
        case TOK_LE:
            return "<=";
        case TOK_GE:
            return ">=";
        case TOK_AND:
            return "&&";
        case TOK_OR:
            return "||";
        case TOK_NOT:
            return "!";
        case TOK_ARROW:
            return "->";
        default:
            return "UNKNOWN";
    }
}

static __inline void lexer_show(const struct Lexer *self)
{
    array_foreach(self->_priv.tokens, Token, t, { printf("[%zu:%zu]\t%-8s\n", t->line, t->col, _to_string(t->kind)); })
}

/**
* private
*/

static void lexer_ctor(void *instance, va_list *args)
{
    Lexer *self = (Lexer *) instance;
    struct _LexerData *priv = &self->_priv;

    self->class = LexerGetClass();
    self->start = lexer_start;
    self->show = lexer_show;
    self->get = lexer_get_tokens;

    priv->io.src = va_arg(*args, const char *);
    priv->tokens = (Array *) new (ArrayClass, sizeof(Token), 16);
    priv->line = 1;
    priv->col = 1;
    priv->pos = 0;
    priv->len = 0;
}

static void lexer_dtor(void __cplus__unused *instance)
{
    /* handled by garbage collector */
}
