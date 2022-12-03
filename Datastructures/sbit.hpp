//#include "../arch/DATATYPE.h"
#include "operations.hpp"
template<class T>
class sbit
{
private:
    T a;
public:
sbit(bool v)
{
    if(v)
        SET_ALL_ONE(a);
    else
        SET_ALL_ZERO(a);
}

T operator &&(T b)
{
a = AND(a,b);
}
T operator ||(T b)
{
a = XOR(a,b);
}
T operator !()
{
return NOT(a);
}



};
