#pragma once
/* #ifdef BOOL */

#include "boost/dynamic_bitset.hpp"
/* #include "operations.hpp" */
#include <sys/types.h>
#define Container boost::dynamic_bitset<uint8_t> 
class Bit_Array
{
private:
    Container a;
public:
Bit_Array(size_t l)
{
    a = alloc_C(l);
}
Bit_Array(char* c, size_t l)
{
    a = alloc_C(c,l);
}


/* boost::dynamic_bitset<unsigned char>::reference operator [](size_t i) */
boost::dynamic_bitset<unsigned char>::reference operator [](size_t i)
{
    return a[i];
}

void operator &&(Container b)
{
    a &= b;
}
void operator ^(Container b)
{
    a ^= b;
}
void operator !()
{
    a = a.flip();
}

Container alloc_C(size_t l)
{
    return boost::dynamic_bitset<uint8_t>(l); 
}

Container alloc_C(char* arr, size_t l)
{
    auto a = boost::dynamic_bitset<uint8_t>(l);  
    boost::from_block_range(arr, arr+l, a);
    return a;
}

void pack(char arr[])
{
    boost::to_block_range(a, arr);
}
/* Container alloc_C(size_t l) */
/* { */
/*     return new u_int64_t; */
/* } */

};

/* #endif */
