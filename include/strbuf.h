#ifndef __LIBCON_STRBUF_H__
#define __LIBCON_STRBUF_H__

#include <stdarg.h>

#ifndef STRBUF_API
#define STRBUF_API
#endif

STRBUF_API char* strfmt(const char* fmt, ...);
STRBUF_API char* strvfmt(const char* fmt, va_list varg);

STRBUF_API void strbuf_init(char** buf, int size);
STRBUF_API void strbuf_free(char** buf);
STRBUF_API void strbuf_clear(char** buf);

STRBUF_API const char* strbuf_fmt(char** buf, const char* fmt, ...);
STRBUF_API const char* strbuf_vfmt(char** buf, const char* fmt, va_list varg);

#endif /* __LIBCON_STRBUF_H__ */

#ifdef STRBUF_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./array.h"

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

void strbuf_init(char** buf, int size)
{
    (void)array_ensure(*buf, size);
}

void strbuf_free(char** buf)
{
    if (buf)
    {
        array_free(*buf);
    }
}

void strbuf_clear(char** buf)
{
    array_raw(*buf)[1] = 0;
}

const char* strbuf_fmt(char** buf, const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    const char* res = strbuf_vfmt(buf, fmt, varg);
    va_end(varg);

    return res;
}

const char* strbuf_vfmt(char** buf, const char* fmt, va_list varg)
{
    va_list varg2;
    va_copy(varg2, varg);
    int n = vsnprintf(NULL, 0, fmt, varg2);
    va_end(varg2);

    int count   = array_count(*buf);
    int reqsize = count + n + 1;
    if (array_ensure(*buf, reqsize))
    {   
        char* str = *buf + count;
        vsnprintf(str, n + 1, fmt, varg);
        return str;
    }
    else
    {
        return NULL;
    }
}

#endif