#include <rune/compiler/ast.h>
#include <rune/compiler/parser/utils.h>

#include <string.h>

const_ bool token_equals_literal(const struct _Token *t, const char *literal)
{
    const size_t len = strlen(literal);

    if (t->length != len) {
        return false;
    }
    return (strncmp(t->value, literal, len) == 0);
}

const_ const struct _Token *peek_tok(struct AST *ast, const size_t cursor)
{
    const size_t total = ast->_tokens->size(ast->_tokens);

    if (cursor >= total) {
        return NULL;
    }
    return (const struct _Token *) ast->_tokens->at(ast->_tokens, cursor);
}

const_ const struct _Token *advance_tok(struct AST *ast, size_t *cursor)
{
    const struct _Token *t = peek_tok(ast, *cursor);

    if (t) {
        *cursor += 1;
    }
    return t;
}
