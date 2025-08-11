#ifndef C_VECTOR_H_
#define C_VECTOR_H_

#include <rune/interface.h>
#include <rune/iterator.h>

#ifndef C_VECTOR_ALIGNMENT
    #define C_VECTOR_ALIGNMENT 32
#endif

struct _VectorData {
    Object *_data;

    size_t _elem_size;
    size_t _size;
    size_t _capacity;

    ObjectDtor _elem_dtor;
};

typedef struct Vector {
    const Class *class;

    void (*push_back)(struct Vector *self, const Object *value);
    any (*at)(const struct Vector *self, const size_t index);

    size_t (*size)(struct Vector *self);

    Iterator *(*begin)(struct Vector *self);
    Iterator *(*end)(struct Vector *self);

    struct _VectorData _priv;
} Vector;

const_ const Class *Vector_getClass(void);
#define VectorClass Vector_getClass()

#define vector_for_each(vec, type, var, code)                                                      \
    for (size_t _i = 0; _i < (vec)->_priv._size; ++_i) {                                           \
        type *var = (type *) vec->at(vec, _i);                                                     \
        code;                                                                                      \
    }

#endif /* C_VECTOR_H_ */
