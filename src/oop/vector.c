#include <mm_malloc.h>
#include <rune/memory.h>
#include <rune/vector.h>
#include <string.h>

static void vector_ctor(Object *self_ptr, va_list *args);
static void vector_dtor(Object *self_ptr);

// clang-format off
static const Class vector_class = {
    .__size__ = sizeof(Vector),
    .__name__ = "Vector",
    .__ctor__ = vector_ctor,
    .__dtor__ = vector_dtor};
// clang-format on

const_ const Class *Vector_getClass(void)
{
    return &vector_class;
}

static inline_ void vector_push_back(Vector *self, const Object *value)
{
    assert(value);
    struct _VectorData *priv = &self->_priv;

    if (priv->_size == priv->_capacity) {

        const size_t new_cap = vectorize(priv->_capacity);
        Object *new_data;

        allocate(new_data, C_VECTOR_ALIGNMENT, new_cap * priv->_elem_size);

        if (priv->_data) {
            memcpy(new_data, priv->_data, priv->_size * priv->_elem_size);
            free(priv->_data);
        }

        priv->_data = new_data;
        priv->_capacity = new_cap;
    }

    Object *dest = (char *) priv->_data + priv->_size * priv->_elem_size;
    memcpy(dest, value, priv->_elem_size);
    ++priv->_size;
}

pure_ static inline_ Object *vector_at(Vector *self, const size_t index)
{
    assert(index < self->_priv._size);
    return (char *) self->_priv._data + index * self->_priv._elem_size;
}

const_ static inline_ size_t vector_size(Vector *self)
{
    return self->_priv._size;
}

pure_ static inline_ Iterator *vector_begin(Vector *self)
{
    struct _VectorData priv = self->_priv;
    Object *end_ptr = (char *) priv._data + priv._size * priv._elem_size;

    return (Iterator *) new (Iterator_getClass(), priv._data, end_ptr, priv._elem_size);
}

pure_ static inline_ Iterator *vector_end(Vector *self)
{
    struct _VectorData priv = self->_priv;
    Object *end_ptr = (char *) priv._data + priv._size * priv._elem_size;

    return (Iterator *) new (Iterator_getClass(), end_ptr, end_ptr, priv._elem_size);
}

static void vector_ctor(Object *self_ptr, va_list *args)
{
    Vector *self = (Vector *) self_ptr;
    struct _VectorData *priv = &self->_priv;

    self->class = Vector_getClass();
    self->push_back = vector_push_back;
    self->at = vector_at;
    self->size = vector_size;
    self->begin = vector_begin;
    self->end = vector_end;

    priv->_elem_size = va_arg(*args, size_t);
    priv->_capacity = va_arg(*args, size_t);
    priv->_size = 0;
    priv->_data = NULL;
    priv->_elem_dtor = va_arg(*args, void (*)(Object *));

    if (priv->_capacity > 0 && priv->_elem_size > 0) {
        allocate(priv->_data, C_VECTOR_ALIGNMENT, priv->_capacity * priv->_elem_size);
    }
}

static void vector_dtor(Object *self_ptr)
{
    Vector *self = (Vector *) self_ptr;
    struct _VectorData *priv = &self->_priv;

    if (priv->_elem_dtor) {

        for (size_t i = 0; i < priv->_size; ++i) {

            Object *element = (char *) priv->_data + i * priv->_elem_size;
            priv->_elem_dtor(element);
        }
    }

    free(priv->_data);
    priv->_capacity = 0;
    priv->_size = 0;
}
