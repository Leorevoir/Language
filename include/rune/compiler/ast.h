#ifndef C_AST_H_
#define C_AST_H_

#include <rune/io.h>
#include <rune/vector.h>

enum ASTNodeType {
    AST_FUNCTION_DECLARATION,
    AST_ASSIGNMENT,
    AST_IF_STATEMENT,
    AST_EXPRESSION,
    AST_CALL
};

struct ASTNode {

    enum ASTNodeType type;

    union {

        struct {
            char *name;
            Vector *body;
        } function_declaration;

        struct {
            char *identifier;
            struct ASTNode *value;
        } assignment;

        struct {
            struct ASTNode *condition;
            Vector *body;
        } if_statement;

        struct {
            char *op;
            struct ASTNode *left;
            struct ASTNode *right;
            char *value;
        } expression;

        struct {
            char *name;
            Vector *args;
        } call;

    } data;
};

typedef struct AST {
    const Class *class;

    void (*collect_tokens)(struct AST *self);
    void (*build)(struct AST *self);
    void (*show)(const struct AST *self);

    struct _IO _io;
    Vector *_tokens;
    Vector *_nodes;
} AST;

const_ const Class *AST_getClass(void);
#define ASTClass AST_getClass()

#endif /* C_AST_H_ */
