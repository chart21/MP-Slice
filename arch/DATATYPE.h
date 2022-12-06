#pragma once
#include "../config.h"
#ifndef RUNTIME
#define RUNTIME
#endif
#ifndef ORTHO
#define ORTHO
#endif
#define US


#if DATTYPE == 1
    #if COMPRESS == 1
        #define BOOL_COMPRESS
        #define NEW(var) new (std::align_val_t(sizeof(uint64_t))) var; //align variables for packing/unpacking
    #else
        #define NEW(var) new var;
    #endif
#endif

#if DATTYPE == 1 
    #include "BOOL.h"
#elif DATTYPE == 8 
    #include "CHAR.h"
#elif DATTYPE == 64 
    #include "STD.h"
#elif DATTYPE == 128 
    #include "SSE.h"
#elif DATTYPE == 256 
    #include "avx.h"
#elif DATTYPE == 512 
    #include "avx512.h"
#endif

