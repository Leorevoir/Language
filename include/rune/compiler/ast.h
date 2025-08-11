#ifndef C_AST_H_
#define C_AST_H_

#include <rune/vector.h>

#include <sys/stat.h>

struct _IO {
    const char *filename;
    const char *buffer;
    struct stat st;
};

typedef struct AST {
    const Class *class;

    void (*collect_tokens)(struct AST *self);

    Vector *_tokens;
    struct _IO _io;
} AST;

const_ const Class *AST_getClass(void);
#define ASTClass AST_getClass()

#endif /* C_AST_H_ */
