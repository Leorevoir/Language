#ifndef C_PLUS_ARRAY_H_
#define C_PLUS_ARRAY_H_

#include <cplus/interface.h>
#include <cplus/macros.h>

struct _ArrayData {
    void *_data;

    size_t _elem_size;
    size_t _size;
    size_t _capacity;

    ClassDtor _elem_dtor;
};

typedef struct Array {
    const Class *class;

    void (*append)(struct Array *self, const void *elem);
    void (*insert)(struct Array *self, size_t index, const void *elem);
    void (*remove)(struct Array *self, size_t index);
    void (*clear)(struct Array *self);
    void (*resize)(struct Array *self, size_t new_size);
    void *(*at)(const struct Array *self, size_t index);

    struct _ArrayData _priv;
} Array;

__cplus__const const Class *ArrayGetClass(void);
#define ArrayClass ArrayGetClass()

#define array_foreach(array, type, var, code)                                                                          \
    for (size_t __i = 0; __i < (array)->_priv._size; ++__i) {                                                          \
        type *var = (type *) array->at(array, __i);                                                                    \
        code;                                                                                                          \
    }

#endif /* C_PLUS_ARRAY_H_ */
