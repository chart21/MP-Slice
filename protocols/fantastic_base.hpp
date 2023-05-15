#pragma once
#include "../arch/DATATYPE.h"
#define SHARE Fantastic_Share
struct Fantastic_Share{
    DATATYPE v0;
    DATATYPE v1;
    DATATYPE v2; 
    DATATYPE verify_store0;// used for saving messages for verification
    #if PARTY > 1
    DATATYPE verify_store1;// used for saving messages for verification
    #endif

Fantastic_Share(){}
Fantastic_Share(DATATYPE v0, DATATYPE v1, DATATYPE v2){
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
}
};

