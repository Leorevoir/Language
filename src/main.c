#include <rune/compiler/ast.h>
#include <rune/memory.h>

int main(void)
{
    auto_clean AST *ast = new (ASTClass, "examples/example.rune");

    ast->collect_tokens(ast);
    ast->build(ast);
    ast->show(ast);

    return 0;
}
