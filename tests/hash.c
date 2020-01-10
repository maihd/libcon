#include <stdio.h>

#include "../include/hash.h"

#include "assets/string_data.c"

int main(int argc, char* argv[])
{
    int i;
    for (i = 0; i < string_data_count; i++)
    {
        unsigned hash = murmur_hash32(string_data[i], strlen(string_data[i]));
        printf("murmur_hash32(\"%s\")=%uu32\n", string_data[i], hash);
    }

    return 0;
}
