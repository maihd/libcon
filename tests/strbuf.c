#include <stdio.h>

#define STRBUF_IMPL
#include "../include/strbuf.h"

int main(int argc, char* argv[])
{
    char* strbuf = 0; 
    // strbuf_init(&strbuf, PREALLOCATE_SIZE);

    const char* strbuf_greeting = strbuf_fmt(&strbuf, "Hello, %s\n", "libcon::strbuf");

    printf("strbuf_greeting: %s\n", strbuf_greeting);

    strbuf_free(&strbuf);
    return 0;
}