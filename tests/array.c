#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/Array.h"

#define intcmp(a, b) ((a) - (b))

int main(int argc, char* argv[])
{
    srand((unsigned long)time(NULL));

    printf("libcon array test.\n");

    Array(int) intarr = Array_make();
    
    int i, n;

    printf("Add 10 randomize numbers... ");
    for (i = 0, n = 10; i < n; i++)
    {
        Array_push(intarr, rand());
    }
    printf("=> DONE!\n");

    Array_free(intarr);
    return 0;
}