#include <cplus/parser.h>
#include <cplus/token.h>

#include "parser_helper.h"

#include <stdio.h>
#include <stdlib.h>

__cplus__used static void parser_ctor(void *instance, va_list *args);
__cplus__used static void parser_dtor(void *instance);

#define there_is_token while (!check(self, TOK_EOF))
#define skip_to_next_statement                                                                                         \
    while (!check(self, TOK_EOF) && !check(self, TOK_SEMI) && !check(self, TOK_DEF) && !check(self, TOK_MODULE)        \
        && !check(self, TOK_STRUCT))

#define skip_brace while (!check(self, TOK_EOF) && !check(self, TOK_RBRACE))
#define skip_parantheses while (!check(self, TOK_EOF) && !check(self, TOK_RPAREN))

static void parse_program(Parser *self);
static void parse_module(Parser *self);
static void parse_struct(Parser *self);
static void parse_function(Parser *self);
static void parse_statement(Parser *self);
static void parse_expression(Parser *self);
static void parse_block(Parser *self);
static void parse_for_statement(Parser *self);
static void parse_forever_statement(Parser *self);
static void parse_if_statement(Parser *self);
static void parse_return_statement(Parser *self);

// clang-format off
static const Class __cplus__used parser_class = {
    .__size__ = sizeof(Parser),
    .__name__ = "Parser",
    .__ctor__ = parser_ctor,
    .__dtor__ = parser_dtor,
};
// clang-format on

/**
 * public
 */

__cplus__const __cplus__used __inline const Class *ParserGetClass(void)
{
    return &parser_class;
}

static void parser_start(Parser *self)
{
    struct _ParserData *priv = &self->_priv;

    __assert(priv->tokens != NULL, "Tokens array cannot be NULL");
    __assert(priv->tokens->_priv._size > 0, "Tokens array cannot be empty");

    priv->current = 0;
    priv->had_error = false;

    parse_program(self);

    __assert(priv->had_error == false, "Failed to parse program");
}

/**
 * parsing functions
 */

/**
 * @syntax: see example/syntax.cp
 */
static void parse_program(Parser *self)
{
    there_is_token
    {
        const Token *token = current_token(self);

        if (token == NULL) {
            break;
        }

        switch (token->kind) {
            case TOK_MODULE:
                parse_module(self);
                break;
            case TOK_DEF:
                parse_function(self);
                break;
            case TOK_STRUCT:
                parse_struct(self);
                break;
            default:
                parse_statement(self);
                break;
        }

        if (self->_priv.had_error) {
            skip_to_next_statement
            {
                advance(self);
            }

            if (check(self, TOK_SEMI)) {
                advance(self);
            }
        }
    }
}

/**
 * @syntax: module <name> { ... }
 */
static void parse_module(Parser *self)
{
    consume(self, TOK_MODULE, "Expected 'module'");
    consume(self, TOK_IDENT, "Expected module name");
    consume(self, TOK_LBRACE, "Expected '{'");

    skip_brace
    {
        const Token *token = current_token(self);

        if (token == NULL) {
            break;
        }

        switch (token->kind) {
            case TOK_STRUCT:
                parse_struct(self);
                break;
            case TOK_DEF:
                parse_function(self);
                break;
            default:
                parse_statement(self);
                break;
        }
    }

    consume(self, TOK_RBRACE, "Expected '}'");
}

/**
 * @syntax: struct <name> { ... }
 *
 * struct Vec2
 * {
 *     x: float;
 *     y: float;
 *
*     def init(@x, @y) -> null
*     {
*     }
 * }
 */
static void parse_struct(Parser *self)
{
    consume(self, TOK_STRUCT, "Expected 'struct'");
    consume(self, TOK_IDENT, "Expected struct name");
    consume(self, TOK_LBRACE, "Expected '{'");

    skip_brace
    {
        if (check(self, TOK_DEF)) {
            parse_function(self);
        } else if (check(self, TOK_IDENT)) {
            advance(self);
            consume(self, TOK_COLON, "Expected ':'");
            consume(self, TOK_IDENT, "Expected type");
            consume(self, TOK_SEMI, "Expected ';'");
        } else {
            error(self, "Expected field or method declaration");
            advance(self);
        }
    }

    consume(self, TOK_RBRACE, "Expected '}'");
}

/**
* @syntax: def <name>(<params>) -> <return_type> { ... }
*
* def add(a: int, b: int) -> int
* {
*     return a + b;
* }
 */
static void parse_function(Parser *self)
{
    consume(self, TOK_DEF, "Expected 'def'");
    consume(self, TOK_IDENT, "Expected function name");
    consume(self, TOK_LPAREN, "Expected '('");

    skip_parantheses
    {
        /** @brief class / struct syntax auto assignment `@` */
        if (check(self, TOK_AT)) {
            advance(self);
            consume(self, TOK_IDENT, "Expected parameter name");
        } else {
            /** @brief regular param syntax */
            consume(self, TOK_IDENT, "Expected parameter name");
            if (check(self, TOK_COLON)) {
                advance(self);
                consume(self, TOK_IDENT, "Expected parameter type");
            }
        }

        /** @brief check for multiple parameters */
        if (check(self, TOK_COMMA)) {
            advance(self);
        } else if (!check(self, TOK_RPAREN)) {
            error(self, "Expected ',' or ')'");
            break;
        }
    }

    consume(self, TOK_RPAREN, "Expected ')'");

    /** @brief return type -> int */
    if (check(self, TOK_ARROW)) {
        advance(self);
        if (check(self, TOK_NULL)) {
            advance(self);
        } else {
            consume(self, TOK_IDENT, "Expected return type");
        }
    }

    parse_block(self);
}

/**
* @syntax: { ... }
*
* parses a block of statements enclosed in braces
*/
static void parse_block(Parser *self)
{
    consume(self, TOK_LBRACE, "Expected '{'");

    skip_brace
    {
        parse_statement(self);
    }

    consume(self, TOK_RBRACE, "Expected '}'");
}

/**
 * @syntax: see example/syntax.cp
 *
 * parses a single statement, which can be a control structure, assignment, or function call
 */
static void parse_statement(Parser *self)
{
    const Token *token = current_token(self);

    if (token == NULL) {
        return;
    }

    switch (token->kind) {
        case TOK_FOR:
            parse_for_statement(self);
            break;
        case TOK_FOREVER:
            parse_forever_statement(self);
            break;
        case TOK_IF:
            parse_if_statement(self);
            break;
        case TOK_RETURN:
            parse_return_statement(self);
            break;
        case TOK_BREAK:
            advance(self);
            if (check(self, TOK_IF)) {
                advance(self);
                parse_expression(self);
            }
            consume(self, TOK_SEMI, "Expected ';'");
            break;
        case TOK_PLUSPLUS:
            advance(self);
            consume(self, TOK_IDENT, "Expected identifier");
            consume(self, TOK_SEMI, "Expected ';'");
            break;
        case TOK_IDENT:
            advance(self);

            /** @brief assignment ... */
            if (check(self, TOK_ASSIGN)) {
                advance(self);
                parse_expression(self);
            } else if (check(self, TOK_LPAREN)) {

                /** @brief ... or function call */
                advance(self);
                while (!check(self, TOK_RPAREN) && !check(self, TOK_EOF)) {
                    parse_expression(self);
                    if (check(self, TOK_COMMA)) {
                        advance(self);
                    } else if (!check(self, TOK_RPAREN)) {
                        break;
                    }
                }
                consume(self, TOK_RPAREN, "Expected ')'");
            }
            consume(self, TOK_SEMI, "Expected ';'");
            break;
        case TOK_LBRACE:
            parse_block(self);
            break;
        default:
            error(self, "Unexpected token in statement");
            advance(self);
            break;
    }
}

//TODO: foreach

/**
 * @syntax: for <var> in <any> { ... }
 *
 *   for i in 10 {
 *       print(i);
 *   }
 *
 *   yes i want number to be iterators too lol
 */
static void parse_for_statement(Parser *self)
{
    consume(self, TOK_FOR, "Expected 'for'");
    consume(self, TOK_IDENT, "Expected loop variable");
    consume(self, TOK_IN, "Expected 'in'");
    parse_expression(self);
    parse_block(self);
}

/**
* @syntax: forever { ... }
*
*   forever {
*       print("Hello, World!");
*   }
*
*   it's like a for (;;) { ... } in C
 */
static void parse_forever_statement(Parser *self)
{
    consume(self, TOK_FOREVER, "Expected 'forever'");
    parse_block(self);
}

/**
 * @syntax: if <condition> { ... } [elsif <condition> { ... }] [else { ... }]
 *
 * if x > 10 {
 *     do something...
 * } elsif x % 2 == 0 {
 *     ...do something else...
 * } else {
 *     ...otherwise
 * }
 *
 * it's like in C but without FORCED ( )
 */
static void parse_if_statement(Parser *self)
{
    consume(self, TOK_IF, "Expected 'if'");
    parse_expression(self);
    parse_block(self);

    while (check(self, TOK_ELSIF)) {
        advance(self);
        parse_expression(self);
        parse_block(self);
    }

    if (check(self, TOK_ELSE)) {
        advance(self);
        parse_block(self);
    }
}

/**
* @syntax: return [<expression>];
*
* return 42;
* return;
*
* it's like in C
 */
static void parse_return_statement(Parser *self)
{
    consume(self, TOK_RETURN, "Expected 'return'");
    if (!check(self, TOK_SEMI)) {
        parse_expression(self);
    }
    consume(self, TOK_SEMI, "Expected ';'");
}

/**
* @syntax: see example/syntax.cp
*
* parses an expression, which can be a variable, literal, or a more complex expression
 */
static void parse_expression(Parser *self)
{
    const Token *token = current_token(self);

    if (token == NULL) {
        return;
    }

    switch (token->kind) {

        case TOK_IDENT:
            advance(self);

            /**
             * @brief handles function calls, member access, and other expressions
             */
            while (check(self, TOK_LPAREN) || check(self, TOK_DOT)) {

                if (check(self, TOK_LPAREN)) {
                    advance(self);

                    skip_parantheses
                    {
                        parse_expression(self);
                        if (check(self, TOK_COMMA)) {
                            advance(self);
                        } else if (!check(self, TOK_RPAREN)) {
                            break;
                        }
                    }

                    consume(self, TOK_RPAREN, "Expected ')'");

                } else if (check(self, TOK_DOT)) {
                    advance(self);
                    consume(self, TOK_IDENT, "Expected member name");
                }
            }
            break;

        case TOK_INTEGER:
        case TOK_FLOAT:
        case TOK_STRING:
        case TOK_TRUE:
        case TOK_FALSE:
        case TOK_NULL:
            advance(self);
            break;

        case TOK_LPAREN:
            advance(self);
            parse_expression(self);
            consume(self, TOK_RPAREN, "Expected ')'");
            break;

        default:
            advance(self);
            break;
    }

    /** @brief handles operators and expressions */
    token = current_token(self);
    if (token
        && (token->kind == TOK_PLUS || token->kind == TOK_MINUS || token->kind == TOK_STAR || token->kind == TOK_SLASH
            || token->kind == TOK_EQ || token->kind == TOK_NEQ || token->kind == TOK_LT || token->kind == TOK_GT
            || token->kind == TOK_LE || token->kind == TOK_GE || token->kind == TOK_AND || token->kind == TOK_OR)) {
        advance(self);
        parse_expression(self);
    }
}

/**
 * private
 */

static void parser_ctor(void *instance, va_list *args)
{
    Parser *self = (Parser *) instance;
    struct _ParserData *priv = &self->_priv;

    self->class = ParserGetClass();
    self->start = parser_start;

    priv->tokens = (const Array *) va_arg(*args, const Array *);
    priv->current = 0;
    priv->had_error = false;
}

static void parser_dtor(void __cplus__unused *instance)
{
    /* handled by garbage collector */
}
