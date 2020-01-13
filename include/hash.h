#pragma once

static unsigned murmur_hash32(const void* data, size_t size)
{
    static unsigned seed = 0;
    
    const unsigned char* key = (const unsigned char*)data;
    if (size > 3)
    {
        const unsigned* key_x4 = (const unsigned*)key;
        size_t i = size >> 2;
        do 
        {
            unsigned k = *key_x4++;

            k *= 0xcc9e2d51;
            k  = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            seed ^= k;
            seed  = (seed << 15) | (seed >> 19);
            seed  = (seed * 5) + 0xe6546b64;
        } while (--i);

        key = (const unsigned char*)key_x4;
    }

    if (size & 3)
    {
        size_t i = size & 3;
        unsigned k = 0;
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
