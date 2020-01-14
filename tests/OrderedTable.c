#include <stdio.h>

#include "../include/OrderedTable.h"

int main(void)
{
    OrderedTable(int) intTable = 0;
    OrderedTable_init(intTable, 64);

    OrderedTable_set(intTable, 10, 10);

    int intValue;
    OrderedTable_get(intTable, 10, 0, &intValue);

    OrderedTable_free(intTable);
    return 0;
}