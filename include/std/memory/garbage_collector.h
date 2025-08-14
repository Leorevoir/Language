#ifndef C_GARBAGE_COLLECTOR_H_
#define C_GARBAGE_COLLECTOR_H_

#include <cplus/interface.h>
#include <cplus/macros.h>
#include <stddef.h>

struct __gc_t {
    size_t size;
    char marked;
    struct __gc_t *next;
};

extern struct __gc_t *__gc_objects;

void __gc_cleanup(void);

#define collect_garbage __gc_cleanup();

void *_new(const Class *class, ...);
void _delete(void *instance);

#define new(class, ...) _new(class, __VA_ARGS__)
#define delete(instance) _delete(instance);

#endif /* C_GARBAGE_COLLECTOR_H_ */
