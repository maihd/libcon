#pragma once

#ifndef ALLOCATOR_API
#define ALLOCATOR_API
#endif

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

#ifdef __cplusplus
extern "C" {
#endif

ALLOCATOR_API Allocator Allocator_newHeap(void);
ALLOCATOR_API void      Allocator_freeHeap(Allocator* allocator);

ALLOCATOR_API Allocator Allocator_newTemp(void* data, int size);
ALLOCATOR_API void      Allocator_freeTemp(Allocator* allocator);

#ifdef __cplusplus
}
#endif

#ifdef ALLOCATOR_IMPL
#include <stdlib.h>

typedef struct AllocatorTempState AllocatorTempState;
struct AllocatorTempState
{
    int     capacity;
    int     allocated;
    char    buffer[];  
};

static void* Allocator_heapAlloc(void* state, int size, int align)
{
    return malloc(size);
}

static void Allocator_heapFree(void* state, void* memoryAddress)
{
    free(memoryAddress);
}

static void* Allocator_tempAlloc(AllocatorTempState* state, int size, int align)
{
    if (state->allocated + size > state->capacity)
    {
        return NULL;
    }

    void* result = &state->buffer[state->allocated];
    state->allocated += size;
    return result;
}

static void Allocator_tempFree(AllocatorTempState* state, void* memoryAddress)
{
}

Allocator Allocator_newHeap(void)
{
    Allocator allocator;
    allocator.state = 0;
    allocator.alloc = Allocator_heapAlloc;
    allocator.free  = Allocator_heapFree;
    return allocator;
}

void Allocator_freeHeap(Allocator* allocator)
{
    if (allocator)
    {
        allocator->state = 0;
        allocator->alloc = 0;
        allocator->free  = 0;
    }
}

Allocator Allocator_newTemp(void* data, int size)
{
    if (!data || size < (int)sizeof(AllocatorTempState))
    {
        Allocator allocator;
        allocator.state = 0;
        allocator.alloc = 0;
        allocator.free  = 0;
        return allocator;
    }

    AllocatorTempState* state = (AllocatorTempState*)data;
    state->capacity = size - sizeof(AllocatorTempState);
    state->allocated = 0;

    Allocator allocator;
    allocator.state = state;
    allocator.alloc = (AllocatorAllocFn)Allocator_tempAlloc;
    allocator.free  = (AllocatorFreeFn)Allocator_tempFree;
    return allocator;
}

void Allocator_freeTemp(Allocator* allocator)
{
    if (allocator)
    {
        allocator->state = 0;
        allocator->alloc = 0;
        allocator->free  = 0;
    }
}

/* END OF ALLOCATOR_IMPL */
#endif
