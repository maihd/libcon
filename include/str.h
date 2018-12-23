#ifndef __LIBCON_STR_H__ 
#define __LIBCON_STR_H__

#include <stdarg.h>

#ifndef STR_API
#define STR_API
#endif

STR_API char* strfmt(const char* fmt, ...);
STR_API char* strvfmt(const char* fmt, va_list varg);

#endif /* __LIBCON_STR_H__ */

#ifdef STR_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strfmt(const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    char* res = strvfmt(fmt, varg);
    va_end(varg);

    return res;
}

char* strvfmt(const char* fmt, va_list varg)
{
    va_list varg2;
    va_copy(varg2, varg);
    int n = vsnprintf(NULL, 0, fmt, varg2);
    va_end(varg2);

    char* str = (char*)malloc(n + 1);
    if (str) vsnprintf(str, n + 1, fmt, varg);
    return str;
}

#endif