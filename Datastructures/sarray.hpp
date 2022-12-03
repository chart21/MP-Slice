#pragma once
#ifndef BOOL
//#include "../arch/DATATYPE.h"
/* #include "operations.hpp" */
#include "../arch/DATATYPE.h"
#include <sys/types.h>
#define Container DATATYPE* 
class Array
{
private:
    Container a;
    size_t length;
public:
Array(size_t l)
{
    length = l;
    a = alloc_C(l);
}
~Array()
{
    delete[] a;
}

DATATYPE* operator [](size_t i)
{
    return a+i;
}
void operator &&(Container b)
{
for(size_t i = 0; i < length; i++)
    a[i] = AND(a[i],b[i]);
}
void operator ^(Container b)
{
for(size_t i = 0; i < length; i++)
    a[i] = XOR(a[i],b[i]);
}
void operator !()
{
for(size_t i = 0; i < length; i++)
    a[i] = NOT(a[i]);
}

/* Container alloc_C(size_t l) */
/* { */
/*     return boost::dynamic_bitset<uint8_t>(l); */ 
/* } */

Container alloc_C(size_t l)
{
    return new DATATYPE;
}

};

#endif
