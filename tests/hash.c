#include <stdio.h>
#include <string.h>

#include "../include/hash.h"

#include "assets/string_data.c"

int main(int argc, char* argv[])
{
    int i;

    printf("Starting testing murmur_hash32\n");
    for (i = 0; i < string_data_count; i++)
    {
        uint32_t hash = murmur_hash32(string_data[i], strlen(string_data[i]));
        printf("murmur_hash32(\"%s\")=%uu32\n", string_data[i], hash);
    }
    printf("\n");

    printf("Starting testing murmur_hash64\n");
    for (i = 0; i < string_data_count; i++)
    {
        uint64_t hash = murmur_hash64(string_data[i], strlen(string_data[i]));
        printf("murmur_hash64(\"%s\")=%luu64\n", string_data[i], hash);
    }
    printf("\n");

    return 0;
}
