#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/array.h"

#define intcmp(a, b) ((a) - (b))

int main(int argc, char* argv[])
{
    srand((unsigned long)time(NULL));

    printf("libcon array test.\n");

    int* intarr = array_make();
    
    int i, n;

    printf("Add 10 randomize numbers... ");
    for (i = 0, n = 10; i < n; i++)
    {
        array_push(intarr, rand());
    }
    printf("=> DONE!\n");

    return 0;
}