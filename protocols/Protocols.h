#pragma once
#include "../config.h"
#define sharemind 1
#define rep3 2
#define astra 3
#define odup 4
#define orep 5
#define ttp3 6
#define ttp4 7
#define PC4_mal 8
#define FantasticFour 9
#define OEC_mal 10
#if PROTOCOL == rep3 
    #define PROTOCOL_LIVE Replicated
    #define PROTOCOL_INIT Replicated_init
    #if INIT == 1 
        #include "replicated_init_template.hpp"
    #endif
    #if LIVE == 1
        #include "replicated_template.hpp"
    #endif
#elif PROTOCOL == sharemind 
    #define PROTOCOL_LIVE Sharemind
    #define PROTOCOL_INIT Sharemind_init
    #if INIT == 1 
        #include "sharemind_init_template.hpp"
    #endif
    #if LIVE == 1
        #include "sharemind_template.hpp"
    #endif
#elif PROTOCOL == astra 
    #if INIT == 1
        #if PARTY == 0
            #define PROTOCOL_INIT OECL0_init
            #include "oecl-P0_init_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_INIT OECL1_init
            #include "oecl-P1_init_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_INIT OECL2_init
            #include "oecl-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == 1 
        #if PARTY == 0
            #define PROTOCOL_LIVE ASTRA0
            #include "astra-P0_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_LIVE ASTRA1
            #include "astra-P1_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_LIVE ASTRA2
            #include "astra-P2_template.hpp"
        #endif
    #endif
#elif PROTOCOL == odup
    #if PRE == 1
        #if PARTY == 0
            #define PROTOCOL_PRE OEC0
            #include "oec-P0_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_PRE -1
        #endif
        #if PARTY == 2
            #define PROTOCOL_PRE -1
        #endif
    #endif
    #if INIT == 1
        #if PARTY == 0
            #define PROTOCOL_INIT OEC0_init
            #include "oec-P0_init_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_INIT OEC1_init
            #include "oec-P1_init_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_INIT OEC2_init
            #include "oec-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == 1 
        #if PARTY == 0
            #if PRE == 1
                #define PROTOCOL_LIVE OEC0_POST
                #include "oec-P0-post_template.hpp"
            #else
                #define PROTOCOL_LIVE OEC0
                #include "oec-P0_template.hpp"
            #endif
        #endif
        #if PARTY == 1
            #define PROTOCOL_LIVE OEC1
            #include "oec-P1_template.hpp"
        #endif
        #if PARTY == 2 
            #define PROTOCOL_LIVE OEC2
            #include "oec-P2_template.hpp"
        #endif
    #endif
#elif PROTOCOL == orep 
    #if PRE == 1
        #if PARTY == 0
            #define PROTOCOL_PRE OECL0
            #include "oecl-P0_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_PRE -1
        #endif
        #if PARTY == 2
            #define PROTOCOL_PRE -1
        #endif
    #endif
    #if INIT == 1
        #if PARTY == 0
            #define PROTOCOL_INIT OECL0_init
            #include "oecl-P0_init_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_INIT OECL1_init
            #include "oecl-P1_init_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_INIT OECL2_init
            #include "oecl-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == 1 
        #if PARTY == 0
            #if PRE == 1
                #define PROTOCOL_LIVE OECL0_POST
                #include "oecl-P0-post_template.hpp"
            #else
                #define PROTOCOL_LIVE OECL0
                #include "oecl-P0_template.hpp"
            #endif
        #endif
        #if PARTY == 1
            #define PROTOCOL_LIVE OECL1
            #include "oecl-P1_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_LIVE OECL2
            #include "oecl-P2_template.hpp"
        #endif
    #endif
#elif PROTOCOL == ttp3 || PROTOCOL == ttp4 || PROTOCOL == PC4_mal || PROTOCOL == FantasticFour
        #define PROTOCOL_LIVE TTP
        #define PROTOCOL_INIT TTP_init
        #if INIT == 1 
            #include "ttp_init_template.hpp"
        #endif
        #if LIVE == 1
            #include "ttp_template.hpp"
        #endif
#elif PROTOCOL == OEC_mal
    /* #define MAL 1 */
    #if PRE == 1
        #if PARTY == 0
            #define PROTOCOL_PRE OEC_MAL0
            #include "oec_mal-P0_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_PRE -1
        #endif
        #if PARTY == 2
            #define PROTOCOL_PRE -1
        #endif
        #if PARTY == 3
            #define PROTOCOL_PRE OEC_MAL3
            #include "oec_mal-P3_template.hpp"
        #endif
#endif
    #if INIT == 1
        #if PARTY == 0
            #define PROTOCOL_INIT OEC_MAL0_init
            #include "oec-mal-P0_init_template.hpp"
        #endif
        #if PARTY == 1
            #define PROTOCOL_INIT OEC_MAL1_init
            #include "oec-mal-P1_init_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_INIT OEC_MAL2_init
            #include "oec-mal-P2_init_template.hpp"
        #endif
        #if PARTY == 3
            #define PROTOCOL_INIT OEC_MAL3_init
            #include "oec-mal-P3_init_template.hpp"
        #endif
    #endif
    #if LIVE == 1 
        #if PARTY == 0
            #if PRE == 1
                #define PROTOCOL_LIVE OEC_MAL0_POST
                #include "oec-mal-P0-post_template.hpp"
            #else
                #define PROTOCOL_LIVE OEC_MAL0
                #include "oec-mal-P0_template.hpp"
            #endif
        #endif
        #if PARTY == 1
            #define PROTOCOL_LIVE OEC_MAL1
            #include "oec-mal-P1_template.hpp"
        #endif
        #if PARTY == 2
            #define PROTOCOL_LIVE OEC_MAL2
            #include "oec-mal-P2_template.hpp"
        #endif
        #if PARTY == 3
            #if PRE == 1
                #define PROTOCOL_LIVE OEC_MAL3_POST
                #include "oec-mal-P3-post_template.hpp"
            #else
                #define PROTOCOL_LIVE OEC_MAL3
                #include "oec-mal-P3_template.hpp"
            #endif
        #endif
    #endif
#endif
