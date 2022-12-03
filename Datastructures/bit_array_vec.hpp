#pragma once
/* #ifdef BOOL */
#include <vector>
/* #include "boost/dynamic_bitset.hpp" */
/* #include "operations.hpp" */
#include <sys/types.h>
#define Container std::vector<bool> 
class Bit_Array
{
private:
    Container a;
public:
Bit_Array(size_t l)
{
    alloc_C(l);
}
Bit_Array(char* begin, char* end, size_t l)
{
    alloc_C(begin,end,l);
}


/* boost::dynamic_bitset<unsigned char>::reference operator [](size_t i) */
std::vector<bool>::reference operator [](size_t i)
{
    return a[i];
}


void alloc_C(size_t l)
{
    a.reserve(l);
}

void alloc_C(char* begin, char* end, size_t l)
{
    a.reserve(l);
    for(size_t i = 0; i < l; i++)
    {
        char mask = 1 << (i % 8);
        a[i] = mask & begin[l/8];
    }
}

void pack(char arr[])
{
    for(size_t i = 0; i < a.size(); i++)
    {
        if(a[i])
        {
            char mask = 1 << (i % 8);
            arr[i/8] += mask;
        }
    }
}
/* Container alloc_C(size_t l) */
/* { */
/*     return new u_int64_t; */
/* } */

};

/* #endif */
