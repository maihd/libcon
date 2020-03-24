#define ALLOCATOR_IMPL

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/Allocator.h"
#include "../include/Array.h"

#define intcmp(a, b) ((a) - (b))

int main(int argc, char* argv[])
{
    int i, n;
    
    printf("libcon array test.\n");

    srand((unsigned long)time(NULL));

    int size = 100 * 1024 * 1024 * sizeof(int);
    void* data = malloc(size);
    Allocator allocator = Allocator_newTemp(data, size);

    Array(int) intArrayWithAllocator = Array_new(int, 0, &allocator);

    printf("Add 1000000 randomize numbers... ");
    for (i = 0, n = 1000000; i < n; i++)
    {
        Array_push(intArrayWithAllocator, rand());
    }
    printf("=> DONE!\n");

    Array_free(intArrayWithAllocator);

    Array(int) intArrayWithoutAllocator = Array_new(int, 0, NULL);

    printf("Add 1000000 randomize numbers without allocator... ");
    for (i = 0, n = 1000000; i < n; i++)
    {
        Array_push(intArrayWithoutAllocator, rand());
    }
    printf("=> DONE!\n");

    Array_free(intArrayWithoutAllocator);

    
    Allocator_freeTemp(&allocator);
    free(data);
    return 0;
}