#ifndef C_MEMORY_H_
#define C_MEMORY_H_

// clang-format off

#include <assert.h>
#include <rune/interface.h>
#include <rune/macros.h>
#include <stdalign.h>
#include <mm_malloc.h>

#ifdef DEBUG
    #include <stdint.h>
    #define check_alignment(ptr, align) assert(((uintptr_t)(ptr) % (align)) == 0)
#else
    #define check_alignment(ptr, align)
#endif

void *_new(const Class *class, ...);
void _delete(Object *obj);
void _auto_delete(Object *obj_ptr);
void _safe_free(Object *ptr);
any _realloc_aligned(any old_ptr, size_t old_size, size_t new_size, size_t align);

/**
 * @brief create a new instance of a class
 */
#define new(c,...) _new(c, __VA_ARGS__)

/**
 * @brief delete an existing instance of a class
 */
#define delete(obj) _delete(obj)

/**
 * @brief free the ptr and set it to NULL at the end of the scope
 */
#define auto_free __attribute__((cleanup(_safe_free)))

/**
 * @brief delete the class, call recursively its dtor & set it to NULL at the end of the scope
 */
#define auto_clean __attribute__((cleanup(_auto_delete)))

/**
 * @brief allocate memory for an object
 * @details if SIMD alignment is required, use posix_memalign otherwise, use malloc bc malloc is less expensive
 */
#define allocate(obj, align, size)                                                       \
    if (align > alignof(void *))                                                         \
        assert(posix_memalign((void **) &obj, align, size) == 0);                        \
    else {                                                                               \
        obj = malloc(size);                                                              \
        assert(obj);                                                                     \
    } \
    check_alignment(obj, align)

#define realloc_aligned(ptr, old_size, new_size, align) \
    _realloc_aligned((any) ptr, old_size, new_size, align)

/**
 * @brief vectorize a value
 */
#define vectorize(x) (x ? x * 2 : 8)

// clang-format on

static inline_ void _defer_null(void *ptr)
{
    Object **pptr = (Object **) ptr;

    *pptr = NULL;
}

/**
* @brief defer nullify a pointer
* @details  set the pointer to NULL at the end of the scope
 */
#define defer_null __attribute__((cleanup(_defer_null)))

#endif /* C_MEMORY_H_ */
