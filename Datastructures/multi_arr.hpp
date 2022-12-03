#include "../arch/DATATYPE.h"
#include <sys/types.h>
#define Container DATATYPE* 
template<int T>
class Multi_Array
{
private:
    Container a;
    size_t dimensions[T];
    size_t curr_Index[T];
    int current_dimension;
    size_t length;
public:
Multi_Array(size_t* d)
{
    dimensions = d;
    length = 0;
    for(int i = 0; i < T; i++)
    {
        length+= d[i];
        curr_Index = 0;
    }
    current_dimension= T-1;
}

Multi_Array operator[](size_t i)
{
    if(curr_Index != -1)
    {
    curr_Index[current_dimension] = i;
    current_dimension -= 1;
    return this;
    }
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
    return NEW(l);
}

};
