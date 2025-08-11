#ifndef C_TOKENS_H_
#define C_TOKENS_H_

#include <stddef.h>

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
