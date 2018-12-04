#include <stdio.h>
#include "../include/array.h"

int main(int argc, char* argv)
{
    printf("libcon array test.\n");

    int* intarr = array_make();
    array_push(intarr, 0);
    array_push(intarr, 1);
    array_push(intarr, 2);
    array_push(intarr, 3);
    array_push(intarr, 4);
    array_push(intarr, 5);
    array_push(intarr, 6);
    array_push(intarr, 7);
    array_push(intarr, 8);
    array_push(intarr, 9);
    
    int i, n;
    printf("Begin loop...\n");
    for (i = 0, n = array_count(intarr); i < n; i++)
    {
        printf("intarr[%d] = %d\n", i, intarr[i]);
    }
    printf("End loop...\n");

    return 0;
}