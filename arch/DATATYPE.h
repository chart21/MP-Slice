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
    #include "AVX.h"
#elif DATTYPE == 512 
    #include "AVX512.h"
#endif

#if num_players == 3
    #if PARTY == 0
        #define P0 2
        #define P1 0
        #define P2 1
        #define PPREV 1
        #define PNEXT 0
    #elif PARTY == 1
        #define P0 0
        #define P1 2
        #define P2 1
        #define PPREV 0
        #define PNEXT 1
    #elif PARTY == 2
        #define P0 0
        #define P1 1
        #define P2 2
        #define PPREV 1
        #define PNEXT 0
    #endif
#elif num_players == 4
    #if PARTY == 0
        #define P0 3
        #define P1 0
        #define P2 1
        #define P3 2
        #define PPREV 2
        #define PNEXT 0
        #define PMIDDLE 1
    #elif PARTY == 1
        #define P0 0
        #define P1 3
        #define P2 1
        #define P3 2
        #define PPREV 0
        #define PNEXT 1
        #define PMIDDLE 2
    #elif PARTY == 2
        #define P0 0
        #define P1 1
        #define P2 3
        #define P3 2
        #define PPREV 1
        #define PNEXT 2
        #define PMIDDLE 0
    #elif PARTY == 3
        #define P0 0
        #define P1 1
        #define P2 2
        #define P3 3
        #define PPREV 2
        #define PNEXT 0
        #define PMIDDLE 1
    #endif
#endif

