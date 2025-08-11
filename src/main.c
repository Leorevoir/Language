#include <rune/compiler/compiler.h>
#include <rune/compiler/parser.h>
#include <rune/memory.h>

int main(void)
{
    auto_clean Parser *parser = new (ParserClass, "examples/example.rune");

    parser->collect_tokens(parser);

    auto_clean Compiler *compiler = new (CompilerClass, parser->_tokens);

    compiler->compile(compiler);
    return 0;
}
