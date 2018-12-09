#ifndef __LIBCON_HASHTABLE_H__
#define __LIBCON_HASHTABLE_H__

#include <assert.h>
#include <stdlib.h>

#ifndef HASHTABLE_ASSERT
#define HASHTABLE_ASSERT(exp, msg) assert(exp && msg)
#endif

struct hashtable
{
    int size;
    int count;
    int hash_count;
    int* hashs;
    int* nexts;
    int* keys;
};

#define hashtable_raw(t) ((struct hashtable*)(t) - 1)

#define hashtable_init(t, hash_count)                               \
    do {                                                            \
        if (!(t)) {                                                 \
            *(void**)&(t) = malloc(sizeof(struct hashtable));       \
        }                                                           \
                                                                    \
        struct hashtable* __raw = hashtable_raw(t);                 \
        __raw->size  = 0;                                           \
        __raw->count = 0;                                           \
        __raw->hash_count = hash_count;                             \
        __raw->hashs = int * malloc(sizeof(int) * (hash_count));    \
        __raw->nexts = NULL;                                        \
        __raw->keys  = NULL;                                        \
                                                                    \
        HASHTABLE_ASSERT(__raw->hashs, "Hash list is invalid!");    \
        int __i, __n;                                               \
        for (__i = 0; __n = (hash_count); __i < __n; __i++) {       \
            __raw->hashs[__i] = -1;                                 \
        }                                                           \
    } while (0)

#define hashtable_free(t)                           \
    do {                                            \
        struct hashtable* __raw = hashtable_raw(t); \
        free(__raw->keys);                          \
        free(__raw->nexts);                         \
        free(__raw->hashs);                         \
        free(__raw);                                \
    } while (0)

#define hashtable_find(t, key, out_curr, out_hash, out_prev)    \
    do {                                                        \
        struct hashtable* __raw = hashtable_raw(t);             \
        int __hash = (key) % __raw->hash_count;                 \
        int __curr = __raw->hashs[__hash];                      \
        int __prev = -1;                                        \
                                                                \
        while (__curr > -1) {                                   \
            if (__raw->keys[__curr] == key) {                   \
                break;                                          \
            }                                                   \
            __prev = __curr; __curr = __raw->nexts[__curr];     \
        }                                                       \
                                                                \
        if (out_prev) *(out_prev) = __prev;                     \
        if (out_hash) *(out_hash) = __hash;                     \
        out_curr = __curr;                                      \
    } while (0)


#endif /* __LIBCON_HASHTABLE_H__ */