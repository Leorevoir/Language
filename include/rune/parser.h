#ifndef C_PARSER_H_
#define C_PARSER_H_

#include <rune/interface.h>
#include <rune/macros.h>
#include <rune/vector.h>

#include <sys/stat.h>

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
    enum _TokenType type;
    char *value;
};

struct _IO {
    const char *filename;
    const char *buffer;
    struct stat st;
};

typedef struct Parser {
    const Class *class;

    void (*collect_tokens)(struct Parser *self);

    Vector *_tokens;
    struct _IO _io;
} Parser;

const_ const Class *Parser_getClass(void);
#define ParserClass Parser_getClass()

#endif /* C_PARSER_H_ */
