#include <std/memory/allocate.h>
#include <std/memory/garbage_collector.h>
#include <stdlib.h>

struct __gc_t *__gc_objects = NULL;

void _delete(void *obj)
{
    void **obj_pp = (void **) &obj;

    if (!obj_pp || !*obj_pp) {
        return;
    }

    const Class *class = *(const Class **) *obj_pp;

    if (class && class->__dtor__) {
        class->__dtor__(*obj_pp);
    }
}

void *_new(const Class *class, ...)
{
    void *obj;
    va_list args;

    allocate(obj, class->__size__);
    va_start(args, class);
    class->__ctor__(obj, &args);
    va_end(args);
    return obj;
}

// clang-format off
void __gc_cleanup(void)
{
    foreach (struct __gc_t, __gc_objects->next, {
        free(it);
    })
}// clang-format on

__cplus__ctor static __cplus__used void __gc_initialize(void)
{
    __gc_objects = malloc(sizeof(struct __gc_t));
    __gc_objects->size = 2;
    __gc_objects->marked = 0;
    __gc_objects->next = NULL;
}

__cplus__dtor static __cplus__used void __gc_finalize(void)
{
    collect_garbage;
    free(__gc_objects);
    __gc_objects = NULL;
}
