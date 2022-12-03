
//#include "../arch/DATATYPE.h"
#include "operations.hpp"
template<class T>
class Array
{
private:
    T a;
    size_t length;
public:
Array(size_t l)
{
    length = l;
}

T operator &&(T b)
{
for(size_t i = 0; i < length; i++)
    a[i] = AND(a[i],b[i]);
}
T operator ||(T b)
{
for(size_t i = 0; i < length; i++)
    a[i] = XOR(a[i],b[i]);
}
T operator !()
{
for(size_t i = 0; i < length; i++)
    a[i] = NOT(a[i]);
}



};
