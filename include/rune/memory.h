#ifndef C_MEMORY_H_
#define C_MEMORY_H_

// clang-format off

#include <assert.h>
#include <rune/interface.h>
#include <rune/macros.h>

void *_new(const Class *class, ...);
void _delete(Object *obj);
void _auto_delete(Object *obj_ptr);
void _safe_free(void *ptr);

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
 * @brief allocate aligned memory and assert on failure
 */
#define allocate(obj, align, size) assert(posix_memalign((void **) &obj, align, size) == 0)

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
