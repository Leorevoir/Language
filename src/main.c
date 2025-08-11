#include <rune/memory.h>
#include <rune/parser.h>

int main(int unused_ argc, const char **argv)
{
    auto_clean Parser *parser = new (ParserClass, argv[1]);

    return 0;
}
