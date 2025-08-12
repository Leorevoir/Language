#include <rune/compiler/ast.h>
#include <rune/compiler/compiler.h>

#include <rune/memory.h>
#include <rune/vector.h>

#include <stddef.h>

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
 * private
 */

struct ByteCodeChunk {
    Vector *code;
    Vector *constants;
};

typedef char *Symbol;

/**
* public
*/

static void compiler_compile(Compiler unused_ *self)
{
    //
}

/**
* private
*/

static void compiler_ctor(Object *self_ptr, va_list *args)
{
    Compiler *self = (Compiler *) self_ptr;
    const Vector *vec = va_arg(*args, const Vector *);

    self->class = Compiler_getClass();
    self->compile = compiler_compile;

    self->_token._data = (const struct _Token *) vec->data(vec);
    self->_token._size = vec->size(vec);
}

static void compiler_dtor(Object unused_ *self_ptr)
{
    /* __nothing__ */
}
