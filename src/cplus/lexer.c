#include "cplus/array.h"
#include <cplus/lexer.h>
#include <std/memory/garbage_collector.h>

__cplus__used static void lexer_ctor(void *instance, va_list *args);
__cplus__used static void lexer_dtor(void *instance);

typedef enum {
    TOK_EOF,
    TOK_ERROR,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,

    TOK_DEF,
    TOK_MODULE,
    TOK_STRUCT,
    TOK_FOR,
    TOK_IN,
    TOK_FOREVER,
    TOK_BREAK,
    TOK_IF,
    TOK_ELSIF,
    TOK_ELSE,
    TOK_RETURN,
    TOK_CONST,
    TOK_DEFER,
    TOK_TRUE,
    TOK_FALSE,
    TOK_NULL,

    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACK,
    TOK_RBRACK,
    TOK_SEMI,
    TOK_COMMA,
    TOK_COLON,
    TOK_DOT,

    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_PERCENT,
    TOK_PLUSPLUS,
    TOK_MINUSMINUS,
    TOK_EQ,
    TOK_ASSIGN,
    TOK_NEQ,
    TOK_LT,
    TOK_GT,
    TOK_LE,
    TOK_GE,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_ARROW,
    TOK_AT
} TokenKind;

typedef struct {
    TokenKind kind;
    char *lexeme;
    size_t line;
    size_t col;
} Token;

// clang-format off
typedef struct { const char *kw; TokenKind k; } Kw;
static const Kw KEYWORDS[] = {
    {"def", TOK_DEF},
    {"module", TOK_MODULE},
    {"struct", TOK_STRUCT},
    {"for", TOK_FOR},
    {"in", TOK_IN},
    {"forever", TOK_FOREVER},
    {"break", TOK_BREAK},
    {"if", TOK_IF},
    {"elsif", TOK_ELSIF},
    {"else", TOK_ELSE},
    {"return", TOK_RETURN},
    {"const", TOK_CONST},
    {"defer", TOK_DEFER},
    {"true", TOK_TRUE},
    {"false", TOK_FALSE},
    {"null", TOK_NULL},
    {NULL, 0}
};

static const Class __cplus__used lexer_class = {
    .__size__ = sizeof(Lexer),
    .__name__ = "Array",
    .__ctor__ = lexer_ctor,
    .__dtor__ = lexer_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used const Class *LexerGetClass(void)
{
    return &lexer_class;
}

/**
 * lexer start
 */

static void lexer_start(Lexer *self)
{
    //
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
        case TOK_NUMBER:
            return "NUMBER";
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
    array_foreach(self->_priv.tokens, Token, t, { printf("[%zu:%zu] %-8s", t->line, t->col, _to_string(t->kind)); })
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

    priv->src = va_arg(*args, const char *);
    priv->tokens = (Array *) new (ArrayClass, sizeof(Token), 16);
}

static void lexer_dtor(void __cplus__unused *instance)
{
    /* __nothing__ */
}
