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

    printf("Print before removing...\n");
    for (i = 0, n = array_count(intarr); i < n; i++)
    {
        printf("intarr[%d] = %d\n", i, intarr[i]);
    }

    i = rand() % array_count(intarr);
    printf("Remove %d from array\n", intarr[i]);
    array_remove(intarr, intarr[i], intcmp);
    printf("Print after removing...\n");
    for (i = 0, n = array_count(intarr); i < n; i++)
    {
        printf("intarr[%d] = %d\n", i, intarr[i]);
    } 

    return 0;
}