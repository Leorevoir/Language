#ifndef C_MEMORY_H_
#define C_MEMORY_H_

#include <rune/macros.h>
#include <stdlib.h>

static inline_ void _auto_free(void *ptr)
{
    free(*(void **) ptr);
}

#define auto_free __attribute__((cleanup(_auto_free)))

#endif
