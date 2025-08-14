#include <cplus/array.h>
#include <std/error/assert.h>
#include <std/memory/allocate.h>

#include <string.h>

__cplus__used static void array_ctor(void *instance, va_list *args);
__cplus__used static void array_dtor(void *instance);

// clang-format off
static const Class __cplus__used array_class = {
    .__size__ = sizeof(Array),
    .__name__ = "Array",
    .__ctor__ = array_ctor,
    .__dtor__ = array_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used const Class *ArrayGetClass(void)
{
    return &array_class;
}

static __inline void array_resize(Array *self, const size_t new_size)
{
    struct _ArrayData *priv = &self->_priv;

    if (new_size > priv->_capacity) {
        const size_t nc = vectorize_size(new_size);
        const size_t ns = nc * priv->_elem_size;

        reallocate(priv->_data, ns);
        priv->_capacity = nc;
    }
}

static __inline void array_append(Array *self, const void *value)
{
    struct _ArrayData *priv = &self->_priv;

    array_resize(self, priv->_size + 1);

    void *dest = (char *) priv->_data + priv->_size * priv->_elem_size;

    memcpy(dest, value, priv->_elem_size);
    ++priv->_size;
}

static __inline void array_insert(Array *self, size_t index, const void *value)
{
    struct _ArrayData *priv = &self->_priv;

    if (index > priv->_size) {
        return;
    }

    array_resize(self, priv->_size + 1);

    void *dest = (char *) priv->_data + index * priv->_elem_size;
    void *src = (char *) priv->_data + index * priv->_elem_size + priv->_elem_size;

    memmove(src, dest, (priv->_size - index) * priv->_elem_size);
    memcpy(dest, value, priv->_elem_size);
    ++priv->_size;
}

static __inline void array_remove(Array *self, size_t index)
{
    struct _ArrayData *priv = &self->_priv;

    if (index >= priv->_size) {
        return;
    }

    void *dest = (char *) priv->_data + index * priv->_elem_size;
    void *src = (char *) priv->_data + (index + 1) * priv->_elem_size;

    memmove(dest, src, (priv->_size - index - 1) * priv->_elem_size);
    --priv->_size;
}

static __inline void array_clear(Array *self)
{
    struct _ArrayData *priv = &self->_priv;

    if (priv->_elem_dtor) {
        for (size_t i = 0; i < priv->_size; ++i) {
            void *elem = (char *) priv->_data + i * priv->_elem_size;
            priv->_elem_dtor(elem);
        }
    }
    priv->_size = 0;
}

static __inline void *array_at(Array *self, const size_t index)
{
    __assert(index < self->_priv._size, "index out of bounds");
    return (char *) self->_priv._data + index * self->_priv._elem_size;
}

/**
* private
*/

static void array_ctor(void *instance, va_list *args)
{
    Array *self = (Array *) instance;
    struct _ArrayData *priv = &self->_priv;

    self->class = ArrayGetClass();
    self->append = array_append;
    self->insert = array_insert;
    self->remove = array_remove;
    self->clear = array_clear;
    self->resize = array_resize;
    self->at = array_at;

    priv->_elem_size = va_arg(*args, size_t);
    priv->_capacity = va_arg(*args, size_t);
    priv->_size = 0;
    priv->_data = NULL;
    priv->_elem_dtor = NULL;

    if (priv->_capacity > 0 && priv->_elem_size > 0) {
        allocate(priv->_data, priv->_capacity * priv->_elem_size);
    }
}

static void array_dtor(void *instance)
{
    Array *self = (Array *) instance;
    struct _ArrayData *priv = &self->_priv;

    if (priv->_elem_dtor) {
        array_foreach(self, any, __obj, { priv->_elem_dtor(__obj); });
    }
}
