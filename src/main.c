#include <rune/compiler/ast.h>
#include <rune/compiler/compiler.h>
#include <rune/memory.h>

int main(void)
{
    auto_clean AST *ast = new (ASTClass, "examples/example.rune");

    ast->collect_tokens(ast);
    // auto_clean Parser *parser = new (ParserClass, "examples/example.rune");
    //
    // parser->collect_tokens(parser);
    //
    // auto_clean Compiler *compiler = new (CompilerClass, ast->_tokens);
    //
    // compiler->compile(compiler);
    return 0;
}
