#ifndef C_PARSER_H_
#define C_PARSER_H_

#include <rune/interface.h>
#include <rune/macros.h>
#include <rune/vector.h>

#include <stdint.h>
#include <sys/stat.h>

// clang-format off

static const char *KEYWORDS[] = {
    "function",
    "if"
    "elif"
    "else"
    "for"
    "foreach"
    "then"
    "do"
};

static const uint64_t NUM_KEYWORDS = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

static const char DELIMITERS[] = {' ', ',', ';', '(', ')', '[', ']', '{', '}', '\0'};

static const char OPERATORS[] = {'+', '-', '*', '/', '>', '<', '=', '%', '\0'};

// clang-format on

enum _TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_INVALID,
    TOKEN_EOF
};

struct _Token {
    enum _TokenType type;
    char *value;
};

struct _IO {
    const char *filename;
    struct stat st;
};

typedef struct Parser {
    const Class *class;

    void (*parse)(struct Parser *self);

    Vector *_tokens;
    struct _IO _io;
} Parser;

const_ const Class *Parser_getClass(void);
#define ParserClass Parser_getClass()

#endif /* C_PARSER_H_ */
