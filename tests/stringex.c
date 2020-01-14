#include <stdio.h>
#include <stdlib.h>

#define STRINGEX_IMPL
#include "../include/stringex.h"

int main(int argc, char* argv[])
{
    char* greeting = strfmt("Hello, %s\n", "libcon");
    printf("greeting: %s\n", greeting);

    free(greeting);
    return 0;
}