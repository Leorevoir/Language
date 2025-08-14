#ifndef C_PLUS_INTERFACE_H_
#define C_PLUS_INTERFACE_H_

#include <stdarg.h>
#include <stddef.h>

typedef void Object;
typedef void *any;

typedef void (*ClassCtor)(Object *instance, va_list *args);
typedef void (*ClassDtor)(Object *instance);

/**
 * @brief base class representation (kind of an insterface)
 */
typedef struct {

    const size_t __size__;
    const char *__name__;

    ClassCtor __ctor__;
    ClassDtor __dtor__;

} Class;

#endif /* C_PLUS_INTERFACE_H_ */
