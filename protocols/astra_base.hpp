 
#pragma once
#include "../arch/DATATYPE.h"
#define Coordinator_Share DATATYPE
struct Evaluator_Share{
    DATATYPE mv;
    DATATYPE lv;
Evaluator_Share(){ }
Evaluator_Share(DATATYPE s1, DATATYPE s2)
{
    mv = s1;
    lv = s2;
}
};

