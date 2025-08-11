#ifndef C_COMPILER_H_
#define C_COMPILER_H_

#include <rune/interface.h>
#include <rune/macros.h>

#include <rune/compiler/tokens.h>

struct _CompilerData {
    const struct _Token *_data;
    size_t _size;
};

typedef struct Compiler {
    const Class *class;

    void (*compile)(struct Compiler *self);
    struct _CompilerData _token;
} Compiler;

const_ const Class *Compiler_getClass(void);
#define CompilerClass Compiler_getClass()

#endif /* C_COMPILER_H_ */
