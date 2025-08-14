#ifndef C_PLUS_PARSER_H_
#define C_PLUS_PARSER_H_

#include <cplus/array.h>
#include <cplus/interface.h>

struct _ParserData {
    const Array *tokens;
};

typedef struct Parser {
    const Class *class;

    void (*start)(struct Parser *self);
    void (*show)(const struct Parser *self);

    struct _ParserData _priv;
} Parser;

__cplus__const const Class *ParserGetClass(void);
#define ParserClass ParserGetClass()

#endif /* C_PLUS_PARSER_H_ */
