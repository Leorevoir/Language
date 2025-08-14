#include "cplus/ast.h"
#include <cplus/lexer.h>
#include <cplus/parser.h>
#include <std/error/assert.h>
#include <std/memory/garbage_collector.h>

int main(const int argc, const char **argv)
{
    __assert((argc >= 2), "USAGE: cplus <file.cp>");

    Lexer *lexer = (Lexer *) new (LexerClass, argv[1]);

    lexer->start(lexer);
    lexer->show(lexer);

    Parser *parser = (Parser *) new (ParserClass, lexer->get(lexer));

    parser->start(parser);

    ASTBuilder *ast = (ASTBuilder *) new (ASTBuilderClass, lexer->get(lexer));

    ast->build(ast);
    ast->show(ast);

    return 0;
}
