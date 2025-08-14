#ifndef C_PLUS_ERROR_ASSERT_H_
#define C_PLUS_ERROR_ASSERT_H_

#include <std/memory/garbage_collector.h>

#include <stdio.h>
#include <stdlib.h>

static __inline void __cplus__assert(const char *file, int line, const char *func, const char *msg)
{
    fprintf(stderr, "ERROR: assertion failed in %s:%d (%s): %s\n", file, line, func, msg);
    collect_garbage;
    exit(84);
}

#define __assert(condition, msg)                                                                                       \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            __cplus__assert(__FILE__, __LINE__, __func__, msg);                                                        \
        }                                                                                                              \
    } while (0)

#endif /* C_PLUS_ERROR_ASSERT_H_ */
