#ifndef C_COMPILER_H_
#define C_COMPILER_H_

#include <rune/interface.h>
#include <rune/macros.h>

#include <rune/compiler/tokens.h>

typedef struct Compiler {
    const Class *class;

    void (*compile)(struct Compiler *self);

    const struct _Token *_tokens;
} Compiler;

const_ const Class *Compiler_getClass(void);
#define CompilerClass Compiler_getClass()

#endif /* C_COMPILER_H_ */
