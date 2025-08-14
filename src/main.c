#include <cplus/lexer.h>
#include <std/memory/garbage_collector.h>

int main(const int argc, const char **argv)
{
    if (argc < 2) {
        return 84;
    }

    Lexer *lexer = (Lexer *) new (LexerClass, argv[1]);

    lexer->show(lexer);

    return 0;
}
