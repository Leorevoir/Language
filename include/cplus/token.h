#ifndef C_PLUS_TOKEN_H_
#define C_PLUS_TOKEN_H_

#include <cplus/array.h>
#include <std/memory/allocate.h>

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    TOK_EOF,
    TOK_ERROR,
    TOK_IDENT,
    TOK_INTEGER,
    TOK_FLOAT,
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
    const char *lexeme;
    size_t line;
    size_t col;
} Token;

typedef struct {
    const Array *tokens;
    size_t current;
    bool had_error;
} Tokenizer;

static __inline Token *_new_token(const TokenKind kind, const char *lexeme, const size_t line, const size_t col)
{
    Token *t;

    allocate(t, sizeof(Token));
    t->kind = kind;
    t->lexeme = lexeme;
    t->line = line;
    t->col = col;
    return t;
}

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


#endif /* C_PLUS_TOKEN_H_ */
