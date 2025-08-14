#ifndef C_PLUS_LEXER_H_
#define C_PLUS_LEXER_H_

#include <cplus/interface.h>
#include <cplus/macros.h>

#include <stdio.h>
#include <sys/stat.h>

typedef struct {
    FILE *stream;
    struct stat st;
} IOFile;

struct _LexerData {
    const char *src;
    const char *buff;

    size_t len;
    size_t pos;
    size_t line;
    size_t col;
};

typedef struct Lexer {
    const Class *class;

    void (*start)(struct Lexer *self);
    struct _LexerData _priv;
} Lexer;

__cplus__const const Class *LexerGetClass(void);
#define LexerClass LexerGetClass()

#endif /* C_PLUS_LEXER_H_ */
