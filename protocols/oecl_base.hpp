#pragma once
#include "../arch/DATATYPE.h"
struct OECL_Share{
    DATATYPE p1;
    DATATYPE p2;
OECL_Share(){ p1 = SET_ALL_ZERO(); p2 = SET_ALL_ZERO(); }
OECL_Share(DATATYPE s1, DATATYPE s2)
{
    p1 = s1;
    p2 = s2;
}
};
