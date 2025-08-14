#include <cplus/lexer.h>

__cplus__used static void lexer_ctor(void *instance, va_list *args);
__cplus__used static void lexer_dtor(void *instance);

// clang-format off
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
* private
*/

static void lexer_ctor(void *instance, va_list *args)
{
    Lexer *self = (Lexer *) instance;
    struct _LexerData *priv = &self->_priv;

    self->class = LexerGetClass();

    priv->src = va_arg(*args, const char *);
}

static void lexer_dtor(void __cplus__unused *instance)
{
    /* __nothing__ */
}
