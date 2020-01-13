#include <stdio.h>

#define STRBUF_IMPL
#include "../include/strbuf.h"

int main(int argc, char* argv[])
{
    char* strbuf = 0; 
    // strbuf_init(&strbuf, PREALLOCATE_SIZE);

    const char* strbuf_greeting = StringBuffer_format(&strbuf, "Hello, %s\n", "libcon::strbuf");

    printf("strbuf_greeting: %s\n", strbuf_greeting);

    StringBuffer_free(&strbuf);
    return 0;
}