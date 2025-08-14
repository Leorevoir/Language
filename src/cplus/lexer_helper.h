#ifndef C_PLUS_l_HELPER_H_
#define C_PLUS_l_HELPER_H_

#include <cplus/lexer.h>
#include <cplus/macros.h>

static __cplus__const __inline char l_peek(const Lexer *L)
{
    if (L->_priv.pos >= L->_priv.len) {
        return '\0';
    }
    return L->_priv.io.buff[L->_priv.pos];
}

static __cplus__const __inline char l_peek_n(const Lexer *L, size_t n)
{
    const size_t p = L->_priv.pos + n;

    if (p >= L->_priv.len) {
        return '\0';
    }
    return L->_priv.io.buff[p];
}

static __inline char l_next(Lexer *L)
{
    if (L->_priv.pos >= L->_priv.len) {
        return '\0';
    }

    const char c = L->_priv.io.buff[L->_priv.pos++];

    if (c == '\n') {
        ++L->_priv.line;
        L->_priv.col = 1;
    } else {
        ++L->_priv.col;
    }
    return c;
}

static __inline void l_skip_whitespace_and_comments(Lexer *L)
{
    forever
    {
        const char c = l_peek(L);

        if (c == '\0') {
            return;
        }
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            l_next(L);
            continue;
        }
        if (c == '/' && l_peek_n(L, 1) == '/') {
            l_next(L);
            l_next(L);
            while (l_peek(L) != '\0' && l_peek(L) != '\n')
                l_next(L);
            continue;
        }
        if (c == '/' && l_peek_n(L, 1) == '*') {
            l_next(L);
            l_next(L);
            forever
            {
                if (l_peek(L) == '\0') {
                    return;
                }

                if (l_peek(L) == '*' && l_peek_n(L, 1) == '/') {
                    l_next(L);
                    l_next(L);
                    break;
                }

                l_next(L);
            }
            continue;
        }
        break;
    }
}

#endif /* C_PLUS_l_HELPER_H_ */
