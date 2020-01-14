#ifndef __LIBCON_STRING_EX_H__ 
#define __LIBCON_STRING_EX_H__

#include <stdarg.h>

#ifndef STRINGEX_API
#define STRINGEX_API
#endif

STRINGEX_API char* strfmt(const char* fmt, ...);
STRINGEX_API char* vstrfmt(const char* fmt, va_list varg);

#endif /* __LIBCON_STRING_EX_H__ */

#ifdef STRINGEX_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strfmt(const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    char* res = vstrfmt(fmt, varg);
    va_end(varg);

    return res;
}

char* vstrfmt(const char* fmt, va_list varg)
{
    static char buffer[4096];

    va_list varg2;
    va_copy(varg2, varg);
    int n = vsnprintf(NULL, 0, fmt, varg2);
    va_end(varg2);

    char* str = n + 1 > sizeof(buffer) ? (char*)malloc(n + 1) : buffer;
    if (str) vsnprintf(str, n + 1, fmt, varg);
    return str;
}

#endif
