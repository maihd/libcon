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

#define hashtable_raw(t)   ((struct hashtable*)(t) - 1)
#define hashtable_size(t)  ((t) ? hashtable_raw(t)->size : 0)
#define hashtable_count(t) ((t) ? hashtable_raw(t)->count : 0)

#define hashtable_init(t, HASH_COUNT)                                               \
    do {                                                                            \
        struct hashtable* _t = (struct hashtable*)malloc(sizeof(struct hashtable)); \
        _t->size  = 0;                                                              \
        _t->count = 0;                                                              \
        _t->hash_count = HASH_COUNT;                                                \
        _t->hashs = (int*)malloc(sizeof(int) * (HASH_COUNT));                       \
        _t->nexts = NULL;                                                           \
        _t->keys  = NULL;                                                           \
                                                                                    \
        HASHTABLE_ASSERT(_t->hashs, "Hash list is invalid!");                       \
        int _i, _n;                                                                 \
        for (_i = 0, _n = (HASH_COUNT); _i < _n; _i++) {                            \
            _t->hashs[_i] = -1;                                                     \
        }                                                                           \
                                                                                    \
        *(void**)&(t) = _t + 1;                                                     \
    } while (0)

#define hashtable_free(t)                           \
    do {                                            \
        struct hashtable* __raw = hashtable_raw(t); \
        free(__raw->keys);                          \
        free(__raw->nexts);                         \
        free(__raw->hashs);                         \
        free(__raw);                                \
    } while (0)

#define hashtable_set(t, key, value)                        \
    do {                                                    \
        int __h, __p;                                       \
        int __c = hashtable_find(t, key, &__h, &__p);       \
        if (__c > -1) {                                     \
            t[__c] = value;                                 \
        } else {                                            \
            int S = hashtable_count(t) + 1;                 \
            if (hashtable_ensure(t, S)) {                   \
                struct hashtable* __raw = hashtable_raw(t); \
                __c = __raw->count++;                       \
                if (__p > -1) {                             \
                    __raw->nexts[__p] = __c;                \
                } else {                                    \
                    __raw->hashs[__h] = __c;                \
                }                                           \
                                                            \
                __raw->nexts[__c]  = -1;                    \
                __raw->keys[__c]   = key;                   \
                (t)[__c] = value;                           \
            }                                               \
        }                                                   \
    } while (0)

#define hashtable_get(t, key, out_value)                    \
    do {                                                    \
        int __c = hashtable_find(t, key, NULL, NULL);       \
        if (__c > -1) {                                     \
            out_value = (t)[__c];                           \
        }                                                   \
    } while (0)

#define hashtable_ensure(t, n) (hashtable_size(t) < (n) ? (*(void**)&(t) = hashtable_grow(t, (n), sizeof((t)[0]))) != NULL : 1)

static int hashtable_find(void* table, int key, int* out_hash, int* out_prev)
{
    struct hashtable* raw = hashtable_raw(table);        
    int hash = (key) % raw->hash_count;            
    int curr = raw->hashs[hash];                 
    int prev = -1;                                   
                                        
    while (curr > -1) 
    {                              
        if (raw->keys[curr] == key) 
        {              
            break;                                     
        }                                              
        prev = curr; 
        curr = raw->nexts[curr];
    }                                                  
                                        
    if (out_prev) *out_prev = prev;                
    if (out_hash) *out_hash = hash;   
    return curr;                                 
}

static void* hashtable_grow(void* table, int reqsize, int elemsize)
{
    if (!table)
    {
        return NULL;
    }

    struct hashtable* raw = hashtable_raw(table);
    int size = raw->size > 0 ? raw->size : 8;
    while (size < reqsize) size *= 2;

    struct hashtable* new_table = (struct hashtable*)realloc(raw, sizeof(struct hashtable) + size * elemsize);
    if (new_table)
    {
        new_table->size  = size;
        new_table->nexts = (int*)realloc(new_table->nexts, size * sizeof(int));
        new_table->keys  = (int*)realloc(new_table->keys, size * sizeof(int));

        if (!new_table->nexts || !new_table->keys)
        {
            free(new_table->nexts);
            free(new_table->keys);
            free(new_table);
            return NULL;
        }
        else
        {
            return new_table + 1;
        }
    }
    else
    {
        free(table);
        return NULL;
    }
}

#endif /* __LIBCON_HASHTABLE_H__ */