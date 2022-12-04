#pragma once
#include <cstddef>
void unpack(char* orig, bool* dest, size_t l)
{    
    for(size_t i = 0; i < l; i++)
    {
        char mask = 1 << (i % 8);
        dest[i] = mask & orig[l/8];
    }
}

void pack(bool* orig, char* dest, size_t l)
{
    for(size_t i = 0; i < l; i++)
    {
            char mask = 1 << (i % 8);
            dest[i/8] += orig[i] * mask;
    }
}

