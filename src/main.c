#include <rune/memory.h>
#include <rune/parser.h>

int main(void)
{
    auto_clean Parser *parser = new (ParserClass, "examples/example.rune");

    parser->collect_tokens(parser);

    return 0;
}
