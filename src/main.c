#include "cplus/array.h"
#include "std/memory/garbage_collector.h"
#include <stdio.h>

typedef struct {
    float x;
    float y;
} vec2f;

static void print_vec2f(const vec2f *v)
{
    printf("vec2f(%f, %f)\n", v->x, v->y);
}

int main(const int argc, const char **argv)
{
    Array *array = (Array *) new (ArrayClass, sizeof(any), 10, NULL);
    const vec2f v1 = {0.1f, 0.2f};

    array->append(array, &v1);
    array->append(array, &(vec2f) {0.3f, 0.4f});
    array->append(array, &(vec2f) {0.5f, 0.6f});

    array_foreach(array, vec2f, v, { print_vec2f(v); });
    delete (array);
}
