#include <cplus/array.h>
#include <cplus/ast.h>
#include <cplus/macros.h>
#include <cplus/token.h>

__cplus__used static void ast_builder_ctor(void *instance, va_list *args);
__cplus__used static void ast_builder_dtor(void *instance);

// clang-format off
static const Class __cplus__used ast_builder_class = {
    .__size__ = sizeof(ASTBuilder),
    .__name__ = "ASTBuilder",
    .__ctor__ = ast_builder_ctor,
    .__dtor__ = ast_builder_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used __inline const Class *ASTBuilderGetClass(void)
{
    return &ast_builder_class;
}

static void ast_builder_show(const ASTBuilder *self)
{
    printf("________________________\n");
    array_foreach(self->tokens, Token, t, { printf("got: %s\n", t->lexeme); });
}

static void ast_builder_build(ASTBuilder *self)
{
    (void) self;
}

/**
* private
*/

static void ast_builder_ctor(void *instance, va_list *args)
{
    ASTBuilder *self = (ASTBuilder *) instance;

    self->class = ASTBuilderGetClass();
    self->show = ast_builder_show;
    self->build = ast_builder_build;
    self->tokens = va_arg(*args, const Array *);
}

static void ast_builder_dtor(void __cplus__unused *instance)
{
    /* handled by garbage collector */
}
