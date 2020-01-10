#pragma once

#include <stdlib.h>
#include <string.h>

/**
 * Array structure
 * struct array_t {
 *      int    size;
 *      int    count;
 *      type_t elements[];
 * }
 */
#define Array(T)                    T*

#define Array_raw(array)            ((int*)(array) - 2)
#define Array_make()                0
#define Array_free(array)           ((array) ? (free(Array_raw(array)), (array) = 0, (void)0) : (void)0)
#define Array_size(array)           ((array) ? Array_raw(array)[0] : 0)
#define Array_count(array)          ((array) ? Array_raw(array)[1] : 0)
#define Array_push(array, value)    (Array_ensure(array, Array_count(array) + 1) ? ((void)((array)[Array_raw(array)[1]++] = value), 1) : 0)
#define Array_pop(array)            ((array)[--Array_raw(array)[1]]);
#define Array_ensure(array, size)   ((!(array) || Array_size(array) < (size)) ? (*((void**)&(array))=Array_grow(array, size, sizeof((array)[0]))) != NULL : 1)

static void* Array_grow(void* array, int reqsize, int elemsize)
{
    int* raw   = array ? Array_raw(array) : NULL;
    int  size  = array && raw[0] > 0 ? raw[0] : 8;
    int  count = Array_count(array);

    while (size < reqsize) size *= 2;

    int* new_array = (int*)realloc(raw, sizeof(int) * 2 + size * elemsize);
    if (new_array)
    {
        new_array[0] = size;
        new_array[1] = count;
        return new_array + 2;
    }
    else
    {
        return NULL;
    }
}
