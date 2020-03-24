#pragma once

#include <stdlib.h>
#include <string.h>

/* Define bool if needed */
#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
typedef char bool;
enum
{
    true = 1, false = 0
};
#endif

/* Make sure we have Allocator */
#ifndef ALLOCATOR_DEFINED
#define ALLOCATOR_DEFINED
typedef void* (*AllocatorAllocFn)(void* state, int size, int align);
typedef void  (*AllocatorFreeFn)(void* state, void* memoryAddress);

typedef struct Allocator Allocator;
struct Allocator
{
    void*               state;
    AllocatorAllocFn    alloc;
    AllocatorFreeFn     free;
};

#define Allocator_alloc(allocator, size, align)     (allocator).alloc((allocator).state, size, align)
#define Allocator_free(allocator, memoryAddress)    (allocator).free((allocator).state, memoryAddress)
#endif

/**
 * ArrayMeta structure
 * Define the information needed by array
 */
typedef struct ArrayMeta
{
    Allocator*  allocator;
    int         size;
    int         count;
    /* T*      elements[]; */
} ArrayMeta;

/**
 * Array<T>
 */
#define Array(T)                                T*

#define Array_new(T, size, allocator)           ((T*)Array_newMemory(size, allocator))
#define Array_free(array)                       ((array) ? (Array_freeMemory(array), (array) = 0, true) : false)

#define Array_getMeta(array)                    ((array) ? (ArrayMeta*)(array) - 1 : 0)
#define Array_getSize(array)                    ((array) ? Array_getMeta(array)->size : 0)
#define Array_getCount(array)                   ((array) ? Array_getMeta(array)->count : 0)

static void* Array_newMemory(int size, Allocator* allocator)
{
    if (size <= 0 && !allocator)
    {
        return NULL;
    }
    else
    {
        int neededMemory = sizeof(ArrayMeta) + (size > 0 ? size : 0);

        ArrayMeta* meta = (ArrayMeta*)(allocator ? Allocator_alloc(*allocator, neededMemory, 0) : malloc(neededMemory));
        meta->allocator = allocator;
        meta->size      = neededMemory - sizeof(ArrayMeta);
        meta->count     = 0;

        return meta + 1;
    }
}

static void Array_freeMemory(void* array)
{
    if (array)
    {
        ArrayMeta* meta = Array_getMeta(array);
        if (meta->allocator)
        {
            Allocator_free(*meta->allocator, meta);
        }
        else
        {
            free(meta);
        }
    }
}

static bool Array_grow(void** array, int targetBufferSize, int itemSize)
{
    int oldSize = Array_getSize(*array);
    int newSize = oldSize ? oldSize : (1 << 4); // 16

    int oldCount = Array_getCount(*array);
    int newCount = oldCount;

    /* New size must be PoTwo and greater than target size */
    while (newSize < targetBufferSize) newSize = newSize << 1;

    /* There is no need to allocate new memory buffer
     * when old buffer is big enough */
    if (newSize <= oldSize)
    {
        return true;
    }
    
    int neededMemory = sizeof(ArrayMeta) + newSize * itemSize;

    ArrayMeta* oldMeta = *array ? Array_getMeta(*array) : NULL;
    ArrayMeta* newMeta = (ArrayMeta*)((oldMeta && oldMeta->allocator) ? Allocator_alloc(*oldMeta->allocator, neededMemory, 0) : malloc(neededMemory));
    if (newMeta)
    {
        newMeta->allocator  = oldMeta ? oldMeta->allocator : NULL;
        newMeta->size       = newSize;
        newMeta->count      = newCount;

        /* Copy the data from old buffer */
        if (oldCount > 0)
        {
            memcpy(newMeta + 1, oldMeta + 1, oldCount * itemSize);
        }

        /* Release unused memory */
        if (oldMeta)
        {
            if (oldMeta->allocator)
            {
                Allocator_free(*oldMeta->allocator, oldMeta);
            }
            else
            {
                free(oldMeta);
            }
        }
        
        *array = newMeta + 1;
        return true;
    }
    else
    {
        return false;
    }
}

/* Define right api for target language */
#ifdef __cplusplus
/* C++ API */
template <typename T>
inline void Array_clear(Array(T) array)
{
    ArrayMeta* meta = Array_getMeta(array);
    if (meta)
    {
        meta->count = 0;
    }
}

template <typename T>
inline bool Array_ensure(Array(T)& array, int size)
{
    if (!(array) || Array_getSize(array) < (size))
    {
        return Array_grow((void**)&(array), size, sizeof((array)[0]));
    }
    else
    {
        return false;
    }
}

template <typename T>
inline bool Array_push(Array(T)& array, T value)
{
    if (Array_ensure(array, Array_getCount(array) + 1))
    {
        ArrayMeta* meta = Array_getMeta(array);
        array[meta->count++] = value;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
inline T Array_pop(Array(T) array)
{
    return array[--Array_getMeta(array)->count];
}

#else
/* C API */

#define Array_clear(array)                      if (array) Array_getMeta(array)->count = 0
#define Array_push(array, value)                (Array_ensure(array, Array_getCount(array) + 1) ? (((array)[Array_getMeta(array)->count++] = value), true) : false)
#define Array_pop(array)                        ((array)[--Array_getMeta(array)->count]);
#define Array_ensure(array, size)               ((!(array) || Array_getSize(array) < (size)) ? Array_grow((void**)&(array), size, sizeof((array)[0])) : true)
#endif

/* END OF FILE */
