#ifndef __CON_ARRAY_H__
#define __CON_ARRAY_H__

/**
 * Array structure
 * struct array_t {
 *      int    size;
 *      int    count;
 *      type_t elements[];
 * }
 */

#include <stdlib.h>
#include <string.h>

#define array_raw(a)         ((int*)(a) - 2)
#define array_make()         0
#define array_free(a)        ((a) ? free(array_raw(a)) : (void)0)
#define array_size(a)        ((a) ? array_raw(a)[0] : 0)
#define array_count(a)       ((a) ? array_raw(a)[1] : 0)
#define array_push(a, v)     (array_ensure(a, array_count(a) + 1) ? ((void)((a)[array_raw(a)[1]++] = v), 1) : 0)
#define array_pop(a, v)      ((a)[--array_raw(a)[1]]);
#define array_ensure(a, n)   ((!(a) || array_size(a) < (n)) ? (*((void**)&(a))=array_grow(a, n, sizeof(a[0]))) != 0 : 1)

#define array_remove(a, v, cmp)                             \
    do {                                                    \
        int _i, _n;                                         \
        for (_i = 0, _n = array_count(a); i < _n; i++) {    \
            if (cmp((a)[_i], v) == 0) {                     \
                array_remove_at(a, _i);                     \
                break;                                      \
            }                                               \
        }                                                   \
    } while (0)

#define array_remove_at(a, i) do { memmove(a + i, a + i + 1, (array_count(a) - _i - 1) * sizeof((a)[0])); array_raw(a)[1]--; } while (0)

static void* array_grow(void* array, int reqsize, int elemsize)
{
    int size = array ? array_raw(array)[0] : 8;
    while (size < reqsize) size *= 2;

    int* new_array = (int*)realloc(array ? array_raw(array) : 0, sizeof(int) * 2 + size * elemsize);
    if (new_array)
    {
        new_array[0] = size;
        if (!array)
        {
            new_array[1] = 0;
        }
        return new_array + 2;
    }
    else
    {
        return array;
    }
}

#endif /* __CON_ARRAY_H__ */