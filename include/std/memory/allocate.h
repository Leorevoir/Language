#ifndef C_MEMORY_ALLOCATE_H_
#define C_MEMORY_ALLOCATE_H_

#include <std/memory/garbage_collector.h>

#include <assert.h>
#include <stdlib.h>

#define __gc_get_ptr(header) ((void *) ((char *) (header) + sizeof(struct __gc_t)))
#define __gc_get_header(ptr) ((struct __gc_t *) ((char *) (ptr) - sizeof(struct __gc_t)))

static __inline void *__gc_allocate(const size_t size)
{
    if (size == 0) {
        return NULL;
    }

    struct __gc_t *header = malloc(sizeof(struct __gc_t) + size);

    header->size = size;
    header->marked = 0;
    header->next = __gc_objects;
    __gc_objects = header;
    return __gc_get_ptr(header);
}

static __inline void *__gc_reallocate(void *ptr, const size_t size)
{
    if (size == 0) {
        free(__gc_get_header(ptr));
        return NULL;
    }

    struct __gc_t *header = __gc_get_header(ptr);
    header = realloc(header, sizeof(struct __gc_t) + size);

    if (!header) {
        return NULL;
    }

    header->size = size;
    return __gc_get_ptr(header);
}

#define allocate(object, size) assert(object = __gc_allocate(size))
#define reallocate(object, size) assert(object = __gc_reallocate(object, size))
#define vectorize_size(x) (x ? x * 2 : 8)

#endif /* C_MEMORY_ALLOCATE_H_ */
