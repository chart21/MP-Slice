#pragma once
#include "../config.h"
#ifndef RUNTIME
#define RUNTIME
#endif
#ifndef ORTHO
#define ORTHO
#endif
#define US


#if (COMPRESS == 1 && DATTYPE == 1)
    #define BOOL_COMPRESS
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

