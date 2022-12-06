#pragma once
#include "../config.h"
#ifndef RUNTIME
#define RUNTIME
#endif
#ifndef ORTHO
#define ORTHO
#endif
#define US


#if (COMPRESS == true && TYPE == bool)
    #define BOOL_COMPRESS
#endif

#if DATTYPE == bool 
    #include "BOOL.h"
#elif DATTYPE == char 
    #include "CHAR.h"
#elif DATTYPE == std 
    #include "STD.h"
#elif DATTYPE == sse 
    #include "SSE.h"
#elif DATTYPE == avx 
    #include "avx.h"
#elif DATTYPE == avx2 
    #include "avx512.h"
#endif
