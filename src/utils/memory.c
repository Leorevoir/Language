#include <rune/memory.h>
#include <stdarg.h>
#include <string.h>

void _safe_free(void *ptr)
{
    defer_null Object **obj_pp = (Object **) ptr;

    if (!obj_pp || !*obj_pp) {
        return;
    }

    free(*obj_pp);
}

/**
* @brief delete implementation
* @details deletes an object & calls its destructor if it exists
*/
void _delete(Object *obj)
{
    Object **obj_pp = (Object **) &obj;

    if (!obj_pp || !*obj_pp) {
        return;
    }

    const Class *class = *(const Class **) *obj_pp;

    if (class && class->__dtor__) {
        class->__dtor__(*obj_pp);
    }

    free(obj);
}

/**
* @brief auto delete implementation
* @details deletes an object at the end of a scope & sets the pointer to NULL
*/
void _auto_delete(void *obj_ptr)
{
    defer_null Object **obj_pp = (Object **) obj_ptr;

    if (!obj_pp || !*obj_pp) {
        return;
    }

    delete (*obj_pp);
}

/**
* @brief create a new instance of a class
* @details allocates memory for the object & calls its constructor with VA_ARGS
*/
void *_new(const Class *class, ...)
{
    Object *obj;
    va_list args;

    allocate(obj, alignof(Object *), class->__size__);
    va_start(args, class);
    class->__ctor__(obj, &args);
    va_end(args);
    return obj;
}

any _realloc_aligned(any old_ptr, size_t old_size, size_t new_size, size_t align)
{
    if (new_size <= old_size) {
        return old_ptr;
    }

    any new_ptr;
    allocate(new_ptr, align, new_size);
    if (old_ptr) {
        memcpy(new_ptr, old_ptr, old_size);
        free(old_ptr);
    }
    return new_ptr;
}
