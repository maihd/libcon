#include <stdio.h>

#include "../include/hash.h"

int main(int argc, char* argv[])
{
    int x = 10;
    int hash_x = murmur_hash(&x, sizeof(x));

    printf("x = %d - hash_x = %d\n", x, hash_x);

    return 0;
}