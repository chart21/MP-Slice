#pragma once
#include "../arch/DATATYPE.h"
#define INIT_SHARE DATATYPE
#define XOR_Share DATATYPE
struct Workaround_Share{
    DATATYPE val_;
    DATATYPE helper_;
Workaround_Share(){}
Workaround_Share(DATATYPE val, DATATYPE helper)
{
    val_ = val;
    helper_ = helper;
}
Workaround_Share(DATATYPE val)
{
    val_ = val;
}
};
