#pragma once
#include "../arch/DATATYPE.h"
#if PARTY == 3
#define SHARE Dealer_Share
#else
#define SHARE OEC_MAL_Share
#endif
struct OEC_MAL_Share{
    DATATYPE v;
    DATATYPE r;
OEC_MAL_Share(){}
OEC_MAL_Share(DATATYPE value, DATATYPE rando)
{
    v = value;
    r = rando;
}
};

struct Dealer_Share{
    DATATYPE r0;
    DATATYPE r1;
    DATATYPE r2;
Dealer_Share(){}
Dealer_Share(DATATYPE r0, DATATYPE r1, DATATYPE r2)
{
    this->r0 = r0;
    this->r1 = r1;
    this->r2 = r2;
}
};