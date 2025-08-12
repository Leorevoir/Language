#ifndef C_PARSER_H_
#define C_PARSER_H_

#include <rune/vector.h>

struct AST;

void parse_program(struct AST *self, size_t *cursor, Vector *nodes);

#endif /* C_PARSER_H_ */
