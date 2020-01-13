#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASHTABLE_ASSERT
#define HASHTABLE_ASSERT(exp, msg) assert((exp) && (msg))
#endif

/* struct HashTable
** types:              int    int       int          int*          int*       unsigned*        T*
** memory layout:   |[size]|[count]|[hashCount]|[---hashs---]|[---nexts---]|[---keys---]|[---values---]|
** memory offset: -12     -8      -4           0         size*4        size*8       size*12        size*sizeof(T)
**/
#define HashTable(T)                T*

#define HashTable_raw(table)        ((int*)(table) - 3)
#define HashTable_size(table)       ((table) ? HashTable_raw(table)[0] : 0)
#define HashTable_count(table)      ((table) ? HashTable_raw(table)[1] : 0)
#define HashTable_hashCount(table)  ((table) ? HashTable_raw(table)[2] : 0)

#define HashTable_calcMemorySize(size, hashCount, itemSize) (3 * sizeof(int) + (size) * (sizeof(int) + sizeof(unsigned) + (itemSize)) + (hashCount) * sizeof(int))

#define HashTable_init(table, size, hashCount)                                                      \
    do {                                                                                            \
        HASHTABLE_ASSERT((size) > 0, "size must be > 0");                                           \
        HASHTABLE_ASSERT((hashCount) > 0, "hashCount must be > 0");                                 \
                                                                                                    \
        int* raw = (int*)malloc(HashTable_calcMemorySize(size, hashCount, sizeof((table)[0])));     \
        raw[0]   = size;                                                                            \
        raw[1]   = 0;                                                                               \
        raw[2]   = hashCount;                                                                       \
                                                                                                    \
        int i, n;                                                                                   \
        int* hashs = raw + 3;                                                                       \
        for (i = 0, n = (hashCount); i < n; i++) {                                                  \
            hashs[i] = -1;                                                                          \
        }                                                                                           \
                                                                                                    \
        *(void**)&(table) = hashs;                                                                  \
    } while (0)

#define HashTable_free(table)                       \
    do {                                            \
        if (table) {                                \
            int* raw = HashTable_raw(table);        \
            free(raw);                              \
            (table) = 0;                            \
        }                                           \
    } while (0)

#define HashTable_set(t, key, value)                                                        \
    do {                                                                                    \
        int hash, prev;                                                                     \
        int curr = HashTable_find(t, key, &hash, &prev);                                    \
        if (curr > -1) {                                                                    \
            void* values = ((int*)(t)) + HashTable_hashCount(t) + 2 * HashTable_size(t);    \
            void* ptr = t;                                                                  \
            *(void**)&(t) = values;                                                         \
            (t)[curr] = value;                                                              \
            *(void**)&(t) = ptr;                                                            \
        } else {                                                                            \
            if (HashTable_ensure(t, HashTable_count(t) + 1)) {                              \
                int* raw = HashTable_raw(t);                                                \
                                                                                            \
                int  size      = raw[0];                                                    \
                int  hashCount = raw[2];                                                    \
                                                                                            \
                int* hashs = (int*)(t);                                                     \
                int* nexts = hashs + hashCount;                                             \
                int* keys  = nexts + size;                                                  \
                                                                                            \
                curr = raw[1]++;                                                            \
                if (prev > -1) {                                                            \
                    nexts[prev] = curr;                                                     \
                } else {                                                                    \
                    hashs[hash] = curr;                                                     \
                }                                                                           \
                                                                                            \
                nexts[curr]  = -1;                                                          \
                keys[curr]   = key;                                                         \
                                                                                            \
                void* values = keys + size;                                                 \
                void* ptr = t;                                                              \
                *(void**)&(t) = values;                                                     \
                (t)[curr] = value;                                                          \
                *(void**)&(t) = ptr;                                                        \
            }                                                                               \
        }                                                                                   \
    } while (0)

#define HashTable_get(table, key, defValue, outValue)                                                   \
    do {                                                                                                \
        HASHTABLE_ASSERT(outValue != 0, "outValue must not be null");                                   \
                                                                                                        \
        int currIndex = HashTable_find(table, key, NULL, NULL);                                         \
        if (currIndex > -1) {                                                                           \
            void* values = ((int*)(table)) + HashTable_hashCount(table) + 2 * HashTable_size(table);    \
            void* ptr = table;                                                                          \
            *(void**)&(table) = values;                                                                 \
            *(outValue) = (table)[currIndex];                                                           \
            *(void**)&(table) = ptr;                                                                    \
        } else {                                                                                        \
            *(outValue) = defValue;                                                                     \
        }                                                                                               \
    } while (0)

#define HashTable_has(table, key) (HashTable_find(table, key, NULL, NULL) > -1)

#define HashTable_ensure(table, n) (HashTable_size(table) < (n) ? (*(void**)&(table) = HashTable_grow(table, (n), sizeof((table)[0]))) != NULL : 1)

static int HashTable_find(void* table, unsigned key, int* outHashIndex, int* outPrevIndex)
{
    if (!table)
    {
        return -1;
    }

    int* raw        = HashTable_raw(table);
    int  size       = raw[0];
    int  hashCount  = raw[2];

    int*        hashs = (int*)table;
    int*        nexts = hashs + hashCount;
    unsigned*   keys  = (unsigned*)(nexts + size);

    int  hashIndex  = key % hashCount;

    int  currIndex  = hashs[hashIndex];                 
    int  prevIndex  = -1;                              
                                        
    while (currIndex > -1) 
    {                              
        if (keys[currIndex] == key) 
        {              
            break;                                     
        }

        prevIndex = currIndex; 
        currIndex = nexts[currIndex];
    }                                                  
                                        
    if (outPrevIndex) *outPrevIndex = prevIndex;                
    if (outHashIndex) *outHashIndex = hashIndex;   
    return currIndex;                                 
}

static void* HashTable_grow(void* table, int targetSize, int itemSize)
{
    if (!table)
    {
        return NULL;
    }

    int* raw = HashTable_raw(table);

    int oldSize = raw[0];
    int newSize = (oldSize > 16 ? oldSize : 16); while (newSize < targetSize) newSize *= 2;
    if (oldSize == newSize)
    {
        return table;
    }

    //int  count      = raw[1];
    int  hashCount  = raw[2];
    
    int* newRaw = (int*)realloc(raw, HashTable_calcMemorySize(newSize, hashCount, itemSize));
    if (newRaw)
    {
        newRaw[0] = newSize;

        int*        oldHashs    = newRaw + 2;
        int*        newHashs    = newRaw + 2;

        int*        oldNexts    = oldHashs + hashCount;
        int*        newNexts    = newHashs + hashCount;

        unsigned*   oldKeys     = (unsigned*)(oldNexts + oldSize);
        unsigned*   newKeys     = (unsigned*)(newNexts + newSize);

        void*       oldValues   = oldKeys + oldSize;
        void*       newValues   = newKeys + newSize;

        memmove(newValues, oldValues, oldSize * itemSize);
        memmove(newKeys, oldKeys, oldSize * sizeof(int));

        return newRaw + 3;
    }
    else
    {
        free(raw);
        return NULL;
    }
}
