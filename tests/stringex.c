#include <stdio.h>

#define STR_IMPL
#include "../include/str.h"

int main(int argc, char* argv[])
{
    char* greeting = strfmt("Hello, %s\n", "libcon");
    printf("greeting: %s\n", greeting);

    free(greeting);
    return 0;
}