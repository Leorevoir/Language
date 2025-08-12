#ifndef C_PRIV_PARSER_UTILS_H
#define C_PRIV_PARSER_UTILS_H

#include <rune/compiler/tokens.h>
#include <rune/macros.h>

#include <stdbool.h>

struct AST;

const_ bool token_equals_literal(const struct _Token *t, const char *literal);

const_ const struct _Token *peek_tok(struct AST *ast, const size_t cursor);
const_ const struct _Token *advance_tok(struct AST *ast, size_t *cursor);

#endif /* C_PRIV_PARSER_UTILS_H */
