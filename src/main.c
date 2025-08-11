#include <rune/compiler/compiler.h>
#include <rune/compiler/parser.h>
#include <rune/memory.h>

int main(void)
{
    auto_clean Parser *ast = new (ParserClass, "examples/example.rune");

    ast->collect_tokens(ast);

    auto_clean Compiler *compiler = new (CompilerClass, ast->_tokens);

    compiler->compile(compiler);
    return 0;
}
