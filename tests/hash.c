#include <stdio.h>

#include "../include/hash.h"

#include "assets/string_data.c"

int main(int argc, char* argv[])
{
    int i;
    for (i = 0; i < string_data_count; i++)
    {
        int hash = murmur_hash(string_data[i], strlen(string_data[i]));
        printf("hashof(\"%s\")=%d\n", string_data[i], hash);
    }

    return 0;
}