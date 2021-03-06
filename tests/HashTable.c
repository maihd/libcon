#include <stdio.h>

#include "../include/hash.h"
#include "../include/HashTable.h"

// test data
#include "./assets/string_data.c"

int main(int argc, char* argv[])
{
    int intValue;

    HashTable(int) intTable = 0;
    HashTable_init(intTable, 64, 64);

    printf("Set value to intTable[10] = 10\n");
    HashTable_set(intTable, 10, 10);
    
    HashTable_get(intTable, 10, -1, &intValue);
    printf("Get value of intTable[10] = %d (-1 mean have no value -> BUGGED)\n", intValue);

    HashTable_free(intTable);

    HashTable(int) hashCollisionCheckTable = 0;
    HashTable_init(hashCollisionCheckTable, 64, 64);
    
    int i;
    int collision = 0;
    for (i = 0; i < string_data_count; i++)
    {
        unsigned key = murmur_hash32(string_data[i], strlen(string_data[1]));
        if (HashTable_has(hashCollisionCheckTable, key))
        {
            collision = 1;
            HashTable_get(hashCollisionCheckTable, key, -1, &intValue);
            printf("Hash collsion. Key: %s - hashedKey: %u - Times: %d\n", string_data[i], key, ++intValue);
        }
        else
        {
            intValue = 1;
        }

        HashTable_set(hashCollisionCheckTable, key, intValue);
    }
    
    if (!collision)
    {
        printf("There is no hash collision occured.\n");
    }

    HashTable_free(hashCollisionCheckTable);
    return 0;
}