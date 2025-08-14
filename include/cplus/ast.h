#ifndef C_PLUS_AST_H_
#define C_PLUS_AST_H_

#include <cplus/array.h>
#include <cplus/token.h>
#include <stdbool.h>

/**
 * AST Node Types
 */

typedef enum {
    AST_PROGRAM,
    AST_MODULE,
    AST_FUNCTION,
    AST_STRUCT,
    AST_PARAMETER,
    AST_FIELD,
    AST_BLOCK,
    AST_IF_STMT,
    AST_FOR_STMT,
    AST_FOREVER_STMT,
    AST_RETURN_STMT,
    AST_BREAK_STMT,
    AST_EXPR_STMT,
    AST_ASSIGNMENT,
    AST_FUNCTION_CALL,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_MEMBER_ACCESS,
    AST_IDENTIFIER,
    AST_LITERAL_INT,
    AST_LITERAL_FLOAT,
    AST_LITERAL_STRING,
    AST_LITERAL_BOOL,
    AST_LITERAL_NULL
} ASTNodeType;

/**
 * Binary operators
 */

typedef enum {
    BOP_ADD,// +
    BOP_SUB,// -
    BOP_MUL,// *
    BOP_DIV,// /
    BOP_EQ, // ==
    BOP_NEQ,// !=
    BOP_LT, // <
    BOP_GT, // >
    BOP_LE, // <=
    BOP_GE, // >=
    BOP_AND,// &&
    BOP_OR  // ||
} BinaryOperator;

/**
 * Unary operators
 */

typedef enum UnaryOperator {
    UOP_MINUS,    // -
    UOP_NOT,      // !
    UOP_PLUSPLUS, // ++
    UOP_MINUSMINUS// --
} UnaryOperator;

/**
 * AST Node data structure
 */

typedef struct ASTNode ASTNode;

typedef struct {
    Array *children;
} ASTProgram;

typedef struct {
    const char *name;
    Array *children;
} ASTModule;

typedef struct {
    const char *name;
    Array *parameters;      // Array of ASTNode* (AST_PARAMETER)
    const char *return_type;// NULL for void/null
    ASTNode *body;          // AST_BLOCK
} ASTFunction;

typedef struct {
    const char *name;
    Array *fields; // Array of ASTNode* (AST_FIELD)
    Array *methods;// Array of ASTNode* (AST_FUNCTION)
} ASTStruct;

typedef struct {
    const char *name;
    const char *type;   // NULL if type inference
    bool is_auto_assign;// true for @param
} ASTParameter;

typedef struct {
    const char *name;
    const char *type;
} ASTField;

typedef struct {
    Array *statements;
} ASTBlock;

typedef struct {
    ASTNode *condition;
    ASTNode *then_block;
    Array *elsif_blocks;// Array of ASTNode* (pairs of condition+block)
    ASTNode *else_block;// NULL if no else block
} ASTIfStatement;

typedef struct {
    const char *variable;
    ASTNode *iterable;
    ASTNode *body;
} ASTForStatement;

typedef struct {
    ASTNode *body;
} ASTForeverStatement;

typedef struct {
    ASTNode *value;// NULL for bare return
} ASTReturnStatement;

typedef struct {
    ASTNode *condition;//NULL for unconditional break
} ASTBreakStatement;

typedef struct {
    ASTNode *expression;
} ASTExpressionStatement;

typedef struct {
    const char *variable;
    ASTNode *value;
} ASTAssignment;

typedef struct {
    ASTNode *function;
    Array *arguments;// Array of ASTNode*
} ASTFunctionCall;

typedef struct {
    BinaryOperator op;
    ASTNode *left;
    ASTNode *right;
} ASTBinaryOperation;

typedef struct {
    UnaryOperator op;
    ASTNode *operand;
} ASTUnaryOperation;

typedef struct {
    ASTNode *object;
    const char *member;
} ASTMemberAccess;

typedef struct ASTIdentifier {
    const char *name;
} ASTIdentifier;

typedef struct {
    int value;
} ASTLiteralInt;

typedef struct {
    float value;
} ASTLiteralFloat;

typedef struct {
    const char *value;
} ASTLiteralString;

typedef struct {
    bool value;
} ASTLiteralBool;

/**
 * main AST node
 */

struct ASTNode {
    ASTNodeType type;
    size_t line;
    size_t column;

    union {
        ASTProgram program;
        ASTModule module;
        ASTFunction function;
        ASTStruct struct_def;
        ASTParameter parameter;
        ASTField field;
        ASTBlock block;
        ASTIfStatement if_stmt;
        ASTForStatement for_stmt;
        ASTForeverStatement forever_stmt;
        ASTReturnStatement return_stmt;
        ASTBreakStatement break_stmt;
        ASTExpressionStatement expr_stmt;
        ASTAssignment assignment;
        ASTFunctionCall function_call;
        ASTBinaryOperation binary_op;
        ASTUnaryOperation unary_op;
        ASTMemberAccess member_access;
        ASTIdentifier identifier;
        ASTLiteralInt literal_int;
        ASTLiteralFloat literal_float;
        ASTLiteralString literal_string;
        ASTLiteralBool literal_bool;
    } data;
};

typedef struct ASTBuilder {
    const Class *class;

    void (*build)(struct ASTBuilder *self);
    void (*show)(const struct ASTBuilder *self);

    Array *nodes;// array of ASTNode*
    Tokenizer _priv;
} ASTBuilder;

__cplus__const const Class *ASTBuilderGetClass(void);
#define ASTBuilderClass ASTBuilderGetClass()

#endif /* C_PLUS_AST_H_ */
