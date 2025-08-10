#include <rune/memory.h>
#include <rune/vector.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} vec2f;

static void print_vec2f(const vec2f *v)
{
    printf("vec2f(%f, %f)\n", (double) v->x, (double) v->y);
}

int main(void)
{
    /**
    * @brief creates a new vector of any elements
    *
    * @param Class -> VectorClass                       the abstraction methods of a Vector
    * @param size_t -> sizeof(any)                      the type of elements to store in the vector (expends to void *)
    * @param size_t -> 1                                the number of elements to preallocate at the instance creation
    * @param ObjectDtor -> NULL                         optional destructor for the elements in the vector
    * 
    */
    Vector *vector = (Vector *) new (VectorClass, sizeof(any), 1, NULL);
    const vec2f v1 = {0.0f, 0.0f};
    const vec2f v2 = {5.0f, 6.0f};

    /**
    * @brief push back elements into the vector
    * @details the first push_back() will not allocate any memory, since the vector was created with a preallocated size of 1.
    */
    vector->push_back(vector, &v1);

    /**
    * @brief this one will allocate memory, since the vector is full and needs to expand
    */
    vector->push_back(vector, &v2);

    /**
    * @brief iterator is an object used to traverse the elements of an iterable container
    * @details to avoid writing "delete(it)" you can use the `auto_clean` macro, which will automatically call `delete(it)` when it goes out of scope
    */
    auto_clean Iterator *it = vector->begin(vector);

    while (it->has_next(it)) {
        print_vec2f((const vec2f *) it->next(it));
    }

    /**
    * @brief ...or you can manually call the Class destructor by using delete(object)
    */
    delete (vector);

    return 0;
}
