#include <rune/iterator.h>
#include <rune/memory.h>

static void iterator_ctor(Object *self_ptr, va_list *args);
static void iterator_dtor(Object *self_ptr);

// clang-format off
static const Class iterator_class = {
    .__size__ = sizeof(Iterator),
    .__name__ = "Iterator",
    .__ctor__ = iterator_ctor,
    .__dtor__ = iterator_dtor
};
// clang-format on

const_ const Class *Iterator_getClass(void)
{
    return &iterator_class;
}

static inline_ Object *iterator_next(Iterator *self)
{
    if (!self->has_next(self)) {
        return NULL;
    }

    struct _IteratorData *priv = &self->_priv;
    Object *ret = priv->_current;

    priv->_current = (char *) priv->_current + priv->_stride;
    return ret;
}

/**
* methods
*/

pure_ static inline_ Object *iterator_get(Iterator *self)
{
    return self->_priv._current;
}

pure_ static inline_ bool iterator_has_next(Iterator *self)
{
    return self->_priv._current < self->_priv._end;
}

static void iterator_ctor(Object *self_ptr, va_list *args)
{
    Iterator *self = (Iterator *) self_ptr;
    struct _IteratorData *priv = &self->_priv;

    self->class = Iterator_getClass();
    self->next = iterator_next;
    self->has_next = iterator_has_next;
    self->get = iterator_get;

    priv->_current = va_arg(*args, Object *);
    priv->_end = va_arg(*args, Object *);
    priv->_stride = va_arg(*args, size_t);
}

static void iterator_dtor(Object unused_ *self_ptr)
{
    /* __nothing__ */
}
