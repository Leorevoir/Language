#ifndef C_TOKENS_H_
#define C_TOKENS_H_

#include <rune/macros.h>
#include <stddef.h>

// clang-format off
static const char unused_ *RUNE_KEYWORDS[] = {
    "function",
    "if",
    "elif",
    "else",
    "for",
    "foreach",
    "then",
    "do"
};

static const char unused_ RUNE_DELIMITERS[] = {' ', ',', ';', '(', ')', '[', ']', '{', '}', '\n', '\0'};

static const char unused_ RUNE_OPERATORS[] = {'+', '-', '*', '/', '>', '<', '=', '%', '\0'};
// clang-format on

enum _TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_INVALID,
    TOKEN_EOF
};

struct _Token {
    const enum _TokenType type;
    const char *value;
    const size_t length;
};

#endif /* C_TOKENS_H_ */
