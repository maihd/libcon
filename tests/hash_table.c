#include <stdio.h>

#include "./assets/string_data.c"
#include "../include/hash.h"
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

    int* hash_collision_table;
    hashtable_init(hash_collision_table, 64);
    
    int i;
    int collision = 0;
    for (i = 0; i < string_data_count; i++)
    {
        int hash = murmur_hash(string_data[i], strlen(string_data[1]));
        if (hashtable_has(hash_collision_table, hash))
        {
            collision = 1;
            hashtable_get(hash_collision_table, hash, val);
            printf("Hash collsion. Key: %s - hash: %d - Times: %d\n", string_data[i], hash, ++val);
        }
        else
        {
            val = 1;
        }

        hashtable_set(hash_collision_table, hash, val);
    }
    
    if (!collision)
    {
        printf("There is no hash collision occured.\n");
    }

    hashtable_free(hash_collision_table);
    return 0;
}