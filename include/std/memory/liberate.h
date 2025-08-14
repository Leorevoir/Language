#ifndef C_MEMORY_LIBERATE_H_
#define C_MEMORY_LIBERATE_H_

#include <cplus/macros.h>
#include <stdlib.h>

static __inline void __gc_null_ptr(void *out_ptr)
{
    void **in_obj = (void **) &out_ptr;

    *in_obj = NULL;
}

#define defer_null __cplus__defer(__gc_null_ptr)

static __inline void __gc_free_ptr(void *out_ptr)
{
    defer_null void *in_ptr = out_ptr;

    if (!in_ptr) {
        return;
    }
    free(in_ptr);
}

#define liberate(ptr) __gc_free_ptr(ptr)

#endif /* C_MEMORY_LIBERATE_H_ */
