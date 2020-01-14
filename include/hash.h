#pragma once

#include <stdint.h>

static uint32_t murmur_hash32(const void* data, uint64_t size)
{
    uint32_t seed = 0;
    
    const uint8_t* key = (const uint8_t*)data;
    if (size > 3)
    {
        const uint32_t* key_x4 = (const uint32_t*)key;
        size_t i = size >> 2;
        do 
        {
            uint32_t k = *key_x4++;

            k *= 0xcc9e2d51;
            k  = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            seed ^= k;
            seed  = (seed << 15) | (seed >> 19);
            seed  = (seed * 5) + 0xe6546b64;
        } while (--i);

        key = (const uint8_t*)key_x4;
    }

    if (size & 3)
    {
        size_t i = size & 3;
        uint32_t k = 0;
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

static uint64_t murmur_hash64(const void* buffer, uint64_t size)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const uint32_t r = 47;

    uint64_t h = 0;

    const uint64_t* data = (const uint64_t*)buffer;
    const uint64_t* end = data + (uint64_t)(size >> 3);

    while (data != end)
    {
#if CPU_BIG_ENDIAN
        uint64_t k = *data++;
        uint8_t* p = (uint8_t*)&k;
        uint8_t c;
        c = p[0]; p[0] = p[7]; p[7] = c;
        c = p[1]; p[1] = p[6]; p[6] = c;
        c = p[2]; p[2] = p[5]; p[5] = c;
        c = p[3]; p[3] = p[4]; p[4] = c;
#else
        uint64_t k = *data++;
#endif

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const uint8_t* data2 = (const uint8_t*)data;

    switch (size & 7)
    {
    case 7: h ^= (uint64_t)(data2[6]) << 48;           /* fall through */
    case 6: h ^= (uint64_t)(data2[5]) << 40;           /* fall through */
    case 5: h ^= (uint64_t)(data2[4]) << 32;           /* fall through */
    case 4: h ^= (uint64_t)(data2[3]) << 24;           /* fall through */
    case 3: h ^= (uint64_t)(data2[2]) << 16;           /* fall through */
    case 2: h ^= (uint64_t)(data2[1]) <<  8;           /* fall through */
    case 1: h ^= (uint64_t)(data2[0]) <<  0; h *= m;   /* fall through */
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
