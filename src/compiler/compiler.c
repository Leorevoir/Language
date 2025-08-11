#include <rune/compiler/compiler.h>

static void compiler_ctor(Object *self_ptr, va_list *args);
static void compiler_dtor(Object *self_ptr);

// clang-format off
static const Class compiler_class = {
    .__size__ = sizeof(Compiler),
    .__name__ = "Compiler",
    .__ctor__ = compiler_ctor,
    .__dtor__ = compiler_dtor
};
// clang-format on

const_ const Class *Compiler_getClass(void)
{
    return &compiler_class;
}

/**
* public
*/

#include <stdio.h>

static void compiler_compile(Compiler *self)
{
    (void) self;
    printf("\n\ncompiler_compile called\n");
}

/**
* private
*/

static void compiler_ctor(Object *self_ptr, va_list *args)
{
    Compiler *self = (Compiler *) self_ptr;

    self->class = Compiler_getClass();
    self->compile = compiler_compile;

    self->_tokens = va_arg(*args, const struct _Token *);
}

static void compiler_dtor(Object unused_ *self_ptr)
{
    /* __nothing__ */
}
