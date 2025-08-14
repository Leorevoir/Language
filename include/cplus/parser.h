#ifndef C_PLUS_PARSER_H_
#define C_PLUS_PARSER_H_

#include <cplus/array.h>
#include <cplus/interface.h>
#include <cplus/token.h>

#include <stdbool.h>

typedef struct Parser {
    const Class *class;

    void (*start)(struct Parser *self);
    Tokenizer _priv;
} Parser;

__cplus__const const Class *ParserGetClass(void);
#define ParserClass ParserGetClass()

#endif /* C_PLUS_PARSER_H_ */
