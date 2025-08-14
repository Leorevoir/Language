#ifndef C_PLUS_PARSER_HELPER_H_
#define C_PLUS_PARSER_HELPER_H_

#include <cplus/macros.h>
#include <cplus/parser.h>
#include <cplus/token.h>

static __cplus__const __inline const Token *current_token(const Parser *self)
{
    const struct _ParserData *priv = &self->_priv;

    if (priv->current >= priv->tokens->_priv._size) {
        return NULL;
    }
    return (Token *) priv->tokens->at(priv->tokens, priv->current);
}

static __cplus__const __inline const Token *peek_token(const Parser *self)
{
    const struct _ParserData *priv = &self->_priv;

    if (priv->current + 1 >= priv->tokens->_priv._size) {
        return NULL;
    }
    return (Token *) priv->tokens->at(priv->tokens, priv->current + 1);
}

static __inline void advance(Parser *self)
{
    struct _ParserData *priv = &self->_priv;

    if (priv->current < priv->tokens->_priv._size) {
        ++priv->current;
    }
}

static __cplus__const __inline bool check(const Parser *self, const TokenKind kind)
{
    const Token *token = current_token(self);

    if (token == NULL) {
        return kind == TOK_EOF;
    }
    return token->kind == kind;
}

static void error(Parser *self, const char *message)
{
    struct _ParserData *priv = &self->_priv;
    const Token *token = current_token(self);

    if (token) {
        fprintf(stderr, "%sPARSE ERROR:%s\n", CPLUS_LOG_RED, CPLUS_LOG_RESET);
        fprintf(stderr, "\t%s%s%s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, message);
        fprintf(stderr, "\tAt line %zu, column %zu\n", token->line, token->col);
    } else {
        fprintf(stderr, "%sPARSE ERROR%s at end of file: %s\n", CPLUS_LOG_RED, CPLUS_LOG_RESET, message);
    }

    priv->had_error = true;
}

static __inline bool match(Parser *self, TokenKind kind)
{
    if (check(self, kind)) {
        advance(self);
        return true;
    }
    return false;
}

static void consume(Parser *self, TokenKind kind, const char *message)
{
    if (check(self, kind)) {
        advance(self);
        return;
    }
    error(self, message);
}

#endif /* C_PLUS_PARSER_HELPER_H_ */
