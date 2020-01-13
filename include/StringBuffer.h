#ifndef __LIBCON_STRBUF_H__
#define __LIBCON_STRBUF_H__

#include <stdarg.h>

#ifndef STRING_BUFFER_API
#define STRING_BUFFER_API
#endif

typedef char* StringBuffer;

STRING_BUFFER_API void StringBuffer_init(StringBuffer* buf, int size);
STRING_BUFFER_API void StringBuffer_free(StringBuffer* buf);
STRING_BUFFER_API void StringBuffer_clear(StringBuffer* buf);

STRING_BUFFER_API const char* StringBuffer_format(StringBuffer* buf, const char* fmt, ...);
STRING_BUFFER_API const char* StringBuffer_vformat(StringBuffer* buf, const char* fmt, va_list varg);

#endif /* __LIBCON_STRBUF_H__ */

#ifdef STRBUF_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./array.h"

void StringBuffer_init(StringBuffer* buf, int size)
{
    (void)Array_ensure(*buf, size);
}

void StringBuffer_free(StringBuffer* buf)
{
    if (buf)
    {
        array_free(*buf);
    }
}

void StringBuffer_clear(StringBuffer* buf)
{
    Array_raw(*buf)[1] = 0;
}

const char* StringBuffer_format(StringBuffer* buf, const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    const char* res = StringBuffer_vformat(buf, fmt, varg);
    va_end(varg);

    return res;
}

const char* StringBuffer_vformat(StringBuffer* buf, const char* fmt, va_list varg)
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
