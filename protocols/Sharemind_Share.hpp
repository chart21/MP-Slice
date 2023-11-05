#pragma once
#include "Share.hpp"
#include "../arch/DATATYPE.h"
class Sharemind_Share : public Share{

public:
    DATATYPE value;
    virtual Share* Not(Share &a) override
    {
        value = NOT(value);
        return this;
    }
};
