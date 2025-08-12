#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include <rune/compiler/ast.h>
#include <rune/compiler/parser/parser.h>
#include <rune/compiler/tokens.h>

#include <rune/string.h>
#include <rune/vector.h>

static void ast_ctor(Object *self_ptr, va_list *args);
static void ast_dtor(Object *self_ptr);

// clang-format off
static const Class ast_class = {
    .__size__ = sizeof(AST),
    .__name__ = "AST",
    .__ctor__ = ast_ctor,
    .__dtor__ = ast_dtor
};
// clang-format on

const_ const Class *AST_getClass(void)
{
    return &ast_class;
}

/**
* helpers
*/

#if defined(DEBUG)
const_ static inline_ const char *token_type_to_string(enum _TokenType type)
{
    switch (type) {
        case TOKEN_EOF:
            return "\033[31mTOKEN_EOF\033[0m";
        case TOKEN_OPERATOR:
            return "TOKEN_OPERATOR";
        case TOKEN_DELIMITER:
            return "\033[32mTOKEN_DELIMITER\033[0m";
        case TOKEN_KEYWORD:
            return "\033[35mTOKEN_KEYWORD\033[0m";
        case TOKEN_IDENTIFIER:
            return "\033[33mTOKEN_IDENTIFIER\033[0m";
        case TOKEN_NUMBER:
            return "\033[34mTOKEN_NUMBER\033[0m";
        case TOKEN_INVALID:
            return "\033[31mTOKEN_INVALID\033[0m";
        default:
            return "UNKNOWN_TOKEN_TYPE";
    }
}
#endif

static inline_ void push_token(Vector **out_tokens, const enum _TokenType type, const char *value,
    const size_t len)
{
    (*out_tokens)
        ->push_back(*out_tokens, &(struct _Token) {.type = type, .value = value, .length = len});
}

/**
* public
*/

static void ast_collect_tokens(AST *self)
{
    size_t left = 0;
    size_t right = 0;
    const struct _IO *io = &self->_io;

    while (right <= (size_t) io->st.st_size && left <= right) {

        /**
         * @brief skip over delimiters
         */
        if (!contains_char(io->buffer[right], RUNE_DELIMITERS)) {
            ++right;
            continue;
        }

        /**
         * @brief if left and right are equal, it means we have a single token
         */
        if (left == right) {

            if (io->buffer[right] == '\0') {
                push_token(&self->_tokens, TOKEN_EOF, io->buffer + right, 0);
                break;
            }
            if (contains_char(io->buffer[right], RUNE_OPERATORS)) {
                push_token(&self->_tokens, TOKEN_OPERATOR, io->buffer + right, 1);
            } else if (contains_char(io->buffer[right], RUNE_DELIMITERS) && io->buffer[right] != ' '
                && io->buffer[right] != '\n') {
                push_token(&self->_tokens, TOKEN_DELIMITER, io->buffer + right, 1);
            }

            ++right;
            left = right;
            continue;
        }

        /**
         * @brief otherwise we have a big token (keyword, identifier, number, etc...)
         */
        const char *substr = io->buffer + left;
        const size_t len = right - left;

        if (len > 0) {
            if (contains_any(substr, len, RUNE_KEYWORDS)) {
                push_token(&self->_tokens, TOKEN_KEYWORD, substr, len);
            } else if (is_digits(substr, len)) {
                push_token(&self->_tokens, TOKEN_NUMBER, substr, len);
            } else if (!isdigit(substr[0]) && !contains_char(substr[0], RUNE_DELIMITERS)) {
                push_token(&self->_tokens, TOKEN_IDENTIFIER, substr, len);
            } else {
                push_token(&self->_tokens, TOKEN_INVALID, substr, len);
            }
        }
        left = right;
    }

#if defined(DEBUG)
    vector_for_each(self->_tokens, struct _Token, __token, {
        printf("token: %.*s\t\ttype: %s\t\tlength: %ld\n", (int) __token->length, __token->value,
            token_type_to_string(__token->type), __token->length);
    });
#endif
}

/**
 * show
 */

static void print_indent(const int depth)
{
    for (int i = 0; i < depth; ++i) {
        putchar(' ');
        putchar(' ');
    }
}

static void print_ast_node(const struct ASTNode *node, const int depth)
{
    if (!node)
        return;

    print_indent(depth);

    switch (node->type) {
        case AST_FUNCTION_DECLARATION:
            printf("FunctionDeclaration: %s\n", node->data.function_declaration.name);
            vector_for_each(node->data.function_declaration.body, struct ASTNode *, child,
                { print_ast_node(*child, depth + 1); });
            break;

        case AST_ASSIGNMENT:
            printf("Assignment: %s\n", node->data.assignment.identifier);
            print_ast_node(node->data.assignment.value, depth + 1);
            break;

        case AST_IF_STATEMENT:
            printf("IfStatement:\n");
            print_indent(depth + 1);
            printf("Condition:\n");
            print_ast_node(node->data.if_statement.condition, depth + 2);
            print_indent(depth + 1);
            printf("Body:\n");
            vector_for_each(node->data.if_statement.body, struct ASTNode *, child,
                { print_ast_node(*child, depth + 2); });
            break;

        case AST_EXPRESSION:
            if (node->data.expression.value) {
                printf("ExpressionValue: %s\n", node->data.expression.value);
            } else {
                printf("ExpressionOp: %s\n", node->data.expression.op);
                print_ast_node(node->data.expression.left, depth + 1);
                print_ast_node(node->data.expression.right, depth + 1);
            }
            break;

        case AST_CALL:
            printf("Call: %s\n", node->data.call.name);
            vector_for_each(node->data.call.args, struct ASTNode *, arg,
                { print_ast_node(*arg, depth + 1); });
            break;

        default:
            printf("Unknown node type\n");
            break;
    }
}

static void ast_show_ast(const AST *self)
{
    vector_for_each(self->_nodes, struct ASTNode *, node, { print_ast_node(*node, 0); });
}

/**
 * build
 */

static void ast_build_ast(AST *self)
{
    size_t cursor = 0;

    parse_program(self, &cursor, self->_nodes);
}

/**
* private
*/

const_ static inline_ bool _does_exist(struct _IO *io)
{
    return (stat(io->filename, &io->st) == 0 && S_ISREG(io->st.st_mode));
}

pure_ static inline_ FILE *_allocate_io(const char *filename)
{
    FILE *file = fopen(filename, "r");
    assert(file != NULL);
    return file;
}

pure_ static inline_ const char *_allocate_buffer(struct _IO *io)
{
    auto_fclose FILE *file = _allocate_io(io->filename);
    char *buffer;
    allocate(buffer, alignof(char), (size_t) io->st.st_size + 1);
    const size_t br = fread(buffer, sizeof(char), (size_t) io->st.st_size, file);
    assert(br != 0 && !feof(file));
    buffer[br] = '\0';
    return buffer;
}

static void ast_ctor(Object *self_ptr, va_list *args)
{
    AST *self = (AST *) self_ptr;
    struct _IO *io = &self->_io;

    self->class = AST_getClass();
    self->collect_tokens = ast_collect_tokens;
    self->build = ast_build_ast;
    self->show = ast_show_ast;
    io->filename = va_arg(*args, char *);
    assert(_does_exist(io) == true);

    io->buffer = _allocate_buffer(io);
    self->_tokens = (Vector *) new (Vector_getClass(), sizeof(struct _Token), 32, NULL);
    self->_nodes = (Vector *) new (Vector_getClass(), sizeof(struct ASTNode *), 32, NULL);

#if defined(DEBUG)
    printf("__BEGIN__\n%s__EOF__\n", io->buffer);
#endif
}

static void ast_dtor(Object *self_ptr)
{
    AST *self = (AST *) self_ptr;

    auto_free const char *buffer = (const char *) self->_io.buffer;

    vector_for_each(self->_nodes, struct ASTNode *, node, { auto_free struct ASTNode *n = *node; });

    delete ((Vector *) self->_nodes);
    delete ((Vector *) self->_tokens);
}
