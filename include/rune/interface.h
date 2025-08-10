#ifndef C_INTERFACE_H_
#define C_INTERFACE_H_

#ifndef Object
    #define Object void
#endif

#ifndef any
    #define any void *
#endif

#include <stdarg.h>
#include <stddef.h>

typedef void (*ObjectCtor)(Object *obj, va_list *args);
typedef void (*ObjectDtor)(Object *obj);

/**
 * @brief base class representation (kind of interface)
 */
typedef struct Class {
    const size_t __size__;
    const char *__name__;

    ObjectCtor __ctor__;
    ObjectDtor __dtor__;
} Class;

#endif /* C_INTERFACE_H_ */
