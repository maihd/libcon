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

#define Array_getMeta(array)        ((int*)(array) - 2)
#define Array_getSize(array)        ((array) ? Array_getMeta(array)[0] : 0)
#define Array_getCount(array)       ((array) ? Array_getMeta(array)[1] : 0)

#define Array_new(T, size)          ((T*)((size) > 0 ? Array_grow(0, size, sizeof(T)) : 0))
#define Array_free(array)           ((array) ? (free(Array_getMeta(array)), (array) = 0, (void)0) : (void)0)

#define Array_clear(array)          if (array) Array_getMeta(array)[1] = 0
#define Array_push(array, value)    (Array_ensure(array, Array_getCount(array) + 1) ? ((void)((array)[Array_getMeta(array)[1]++] = value), 1) : 0)
#define Array_pop(array)            ((array)[--Array_getMeta(array)[1]]);
#define Array_ensure(array, size)   ((!(array) || Array_getSize(array) < (size)) ? (*((void**)&(array))=Array_grow(array, size, sizeof((array)[0]))) != NULL : 1)

static void* Array_grow(void* array, int reqsize, int elemsize)
{
    int oldSize = Array_getSize(array);
    int newSize = oldSize ? oldSize : 16;
    while (newSize < reqsize) newSize = newSize << 1;
    if (newSize <= oldSize)
    {
        return array;
    }

    int oldCount = Array_getCount(array);
    int newCount = oldCount;

    int* oldMeta = array ? Array_getMeta(array) : NULL;
    int* newMeta = realloc(oldMeta, 2 * sizeof(int) + newSize * elemsize);
    if (newMeta)
    {
        newMeta[0] = newSize;
        newMeta[1] = newCount;
        return newMeta + 2;
    }
    else
    {
        free(oldMeta);
        return NULL;
    }
}
