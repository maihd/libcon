#pragma once 

#include <stdlib.h>
#include <string.h>

/* struct OrderedTable
** types:              int    int        int*          T*
** memory layout:   |[size]|[count]|[---keys---]|[---values---]|
** memory offset:  -8     -4       0        size*4        
**/
#define OrderedTable(T) T*

#define OrderedTable_getMeta(table)     ((int*)(table) - 2)
#define OrderedTable_getSize(table)     ((table) ? OrderedTable_getMeta(table)[0] : 0)
#define OrderedTable_getCount(table)    ((table) ? OrderedTable_getMeta(table)[1] : 0)

#define OrderedTable_ensure(table, n)   (OrderedTable_getSize(table) < (n) ? (*(void**)&(table) = OrderedTable_grow(table, (n), sizeof((table)[0]))) != NULL : 1)

#define OrderedTable_init(table, capacity) (void)OrderedTable_ensure(table, capacity)

#define OrderedTable_free(table)                            \
    do {                                                    \
        if (table) {                                        \
            int* meta = OrderedTable_getMeta(table);        \
            free(meta);                                     \
            (table) = 0;                                    \
        }                                                   \
    } while (0)

#define OrderedTable_set(table, key, value)                                                             \
    do {                                                                                                \
        int index = OrderedTable_find(table, key);                                                      \
        if (index > -1)                                                                                 \
        {                                                                                               \
            void* values = ((int*)(table)) + OrderedTable_getSize(table);                               \
            void* ptr = table;                                                                          \
            *(void**)&(table) = values;                                                                 \
            (table)[index] = value;                                                                     \
            *(void**)&(table) = ptr;                                                                    \
        }                                                                                               \
        else if (OrderedTable_ensure(table, OrderedTable_getCount(table) + 1))                          \
        {                                                                                               \
            index = OrderedTable_getMeta(table)[1]++;                                                   \
            ((unsigned*)(table))[index] = key;                                                          \
                                                                                                        \
            void* values = ((char*)(table)) + OrderedTable_getSize(table) * sizeof(unsigned);           \
            void* ptr = table;                                                                          \
            *(void**)&(table) = values;                                                                 \
            (table)[index] = value;                                                                     \
            *(void**)&(table) = ptr;                                                                    \
        }                                                                                               \
    } while (0)

#define OrderedTable_get(table, key, defValue, outValue)                                                \
    do {                                                                                                \
        int currIndex = OrderedTable_find(table, key);                                                  \
        if (currIndex > -1) {                                                                           \
            void* values = ((int*)(table)) + OrderedTable_getSize(table);                               \
            void* ptr = table;                                                                          \
            *(void**)&(table) = values;                                                                 \
            *(outValue) = (table)[currIndex];                                                           \
            *(void**)&(table) = ptr;                                                                    \
        } else {                                                                                        \
            *(outValue) = defValue;                                                                     \
        }                                                                                               \
    } while (0)

#define HashTable_has(table, key) (HashTable_find(table, key) > -1)

static void* OrderedTable_grow(void* table, int size, int itemSize)
{
    int oldSize = OrderedTable_getSize(table);
    int newSize = oldSize > 16 ? oldSize : 16;
    while (newSize < size) newSize = newSize << 1;
    if (oldSize == newSize)
    {
        return table;
    }

    int oldCount = OrderedTable_getCount(table);
    int newCount = oldCount;

    int* oldMeta = table ? OrderedTable_getMeta(table) : NULL;
    int* newMeta = realloc(oldMeta, sizeof(int) * 2 + newSize * (sizeof(unsigned) + itemSize));
    if (newMeta)
    {
        if (oldSize > 0)
        {
            void* oldKeys = newMeta + 2;
            void* newKeys = newMeta + 2;

            void* oldValues = (char*)oldKeys + oldSize * sizeof(unsigned);
            void* newValues = (char*)newKeys + newSize * sizeof(unsigned);

            memmove(newValues, oldValues, oldSize * itemSize);
        }

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

static int OrderedTable_find(void* table, unsigned key)
{
    unsigned* keys = table; 

    for (int i = 0, n = OrderedTable_getCount(table); i < n; i++)
    {
        if (keys[i] == key)
        {
            return i;
        }
    }

    return -1;
}
