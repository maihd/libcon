#include <stdio.h>

#include "../include/hash_table.h"

int main(int argc, char* argv[])
{
    int* itab;
    hashtable_init(itab, 64);

    printf("Set value to itab[10] = 10\n");
    hashtable_set(itab, 10, 10);
    
    int val;
    printf("Get value of itab[10]\n");
    hashtable_get(itab, 10, val);
    printf("val = %d\n", val);

    hashtable_free(itab);
    return 0;
}