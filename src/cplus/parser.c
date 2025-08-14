#include <cplus/parser.h>
#include <cplus/token.h>

#include "parser_helper.h"

__cplus__used static void parser_ctor(void *instance, va_list *args);
__cplus__used static void parser_dtor(void *instance);

// clang-format off
static const Class __cplus__used parser_class = {
    .__size__ = sizeof(Parser),
    .__name__ = "Parser",
    .__ctor__ = parser_ctor,
    .__dtor__ = parser_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used __inline const Class *ParserGetClass(void)
{
    return &parser_class;
}

static void parser_start(Parser *self)
{
    struct _ParserData *priv = &self->_priv;

    //
}

static void parser_show(const Parser *self)
{
    const struct _ParserData *priv = &self->_priv;

    if (priv->tokens == NULL) {
        return;
    }
}

/**
* private
*/

static void parser_ctor(void *instance, va_list *args)
{
    Parser *self = (Parser *) instance;
    struct _ParserData *priv = &self->_priv;

    self->class = ParserGetClass();
    self->start = parser_start;
    self->show = parser_show;

    self->_priv.tokens = (const Array *) va_arg(*args, const Array *);
}

static void parser_dtor(void __cplus__unused *instance)
{
    /* handled by garbage collector */
}
