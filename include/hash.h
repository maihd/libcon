#ifndef __LIBCON_HASH_H__
#define __LIBCON_HASH_H__

#include <string.h>

#if defined(__cplusplus)
#define HAVE_HASHOF
inline int hashof(const char* x)
{
    return murmur_hash(x, strlen(x));
}

template <typename type_t>
inline int hashof(const type_t& x)
{
    return murmur_hash(&x, sizeof(x));
}
#endif

static int murmur_hash(const void* data, size_t size)
{
    static int seed = 0;
    
    const char* key = (const char*)data;
    if (size > 3)
    {
        const int* key_x4 = (const int*)key;
        size_t i = size >> 2;
        do 
        {
            int k = *key_x4++;

            k *= 0xcc9e2d51;
            k  = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            seed ^= k;
            seed  = (seed << 15) | (seed >> 19);
            seed  = (seed * 5) + 0xe6546b64;
        } while (--i);

        key = (const char*)key_x4;
    }

    if (size & 3)
    {
        size_t i = size & 3;
        int k = 0;
        key = &key[i - 1];

        do 
        {
            k = k << 8;
            k = k | (*key--);
        } while (--i);

        k *= 0xcc9e2d51;
        k  = (k << 15) | (k >> 17);
        k *= 0x1b873593;

        seed ^= k;
    }

    seed ^= size;
    seed ^= seed >> 16;
    seed *= 0x85ebca6b;
    seed ^= seed >> 13;
    seed *= 0xc2b2ae35;
    seed ^= seed >> 16;

    return seed;
}

#endif /* __LIBCON_HASH_H__ */