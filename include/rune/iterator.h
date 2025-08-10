#ifndef C_ITERATOR_H_
#define C_ITERATOR_H_

#include <rune/interface.h>
#include <rune/macros.h>
#include <stdbool.h>

struct _IteratorData {
    Object *_current;
    Object *_end;
    size_t _stride;
};

typedef struct Iterator {
    const Class *class;

    Object *(*next)(struct Iterator *self);
    Object *(*get)(struct Iterator *self);
    bool (*has_next)(struct Iterator *self);

    struct _IteratorData _priv;
} Iterator;

const_ const Class *Iterator_getClass(void);
#define IteratorClass Iterator_getClass()

#endif /* C_ITERATOR_H_ */
