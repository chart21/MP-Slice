#pragma once
#include "../config.h"
#if PROTOCOL == rep 
    #define PROTOCOL_LIVE Replicated
    #define PROTOCOL_INIT Replicated_init
    #if INIT == true 
        #include "replicated_init_template.hpp"
    #endif
    #if LIVE == true
        #include "replicated_template.hpp"
    #endif
#elif PROTOCOL == sharemind 
    #define PROTOCOL_LIVE Sharemind
    #define PROTOCOL_INIT Sharemind_init
    #if INIT == true 
        #include "sharemind_init_template.hpp"
    #endif
    #if LIVE == true
        #include "sharemind_template.hpp"
    #endif
#elif PROTOCOL == astra 
    #if INIT == true 
        #if PARTY == 0 || Party == all
            #define PROTOCOL_INIT OECL0_init
            #include "oecl-P0_init_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_INIT OECL1_init
            #include "oecl-P1_init_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_INIT OECL2_init
            #include "oecl-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == true 
        #if PARTY == 0 || Party == all
            #define PROTOCOL_LIVE ASTRA0
            #include "astra-P0_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_LIVE ASTRA1
            #include "astra-P1_template.hpp"
        #endif
        #if PARTY == 2 || Party == all
            #define PROTOCOL_LIVE ASTRA2
            #include "astra-P2_template.hpp"
        #endif
    #endif
#elif PROTOCOL == oec-dup
    #if INIT == true
        #if PARTY == 0 || Party == all
            #define PROTOCOL_INIT OEC0_init
            #include "oec-P0_init_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_INIT OEC1_init
            #include "oec-P1_init_template.hpp"
        #endif
        #if PARTY == 2 || Party == all
            #define PROTOCOL_INIT OEC2_init
            #include "oec-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == true 
        #if PARTY == 0 || Party == all
            #define PROTOCOL_LIVE OEC0
            #include "oec-P0_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_LIVE OEC1
            #include "oec-P1_template.hpp"
        #endif
        #if PARTY == 2 || Party == all
            #define PROTOCOL_LIVE OEC2
            #include "oec-P2_template.hpp"
        #endif
    #endif
#elif PROTOCOL == oec-rep 
    #if init == true
        #if PARTY == 0 || Party == all
            #define PROTOCOL_INIT OECL0_init
            #include "oecl-P0_init_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_INIT OECL1_init
            #include "oecl-P1_init_template.hpp"
        #endif
        #if PARTY == 2 || Party == all
            #define PROTOCOL_INIT OECL2_init
            #include "oecl-P2_init_template.hpp"
        #endif
    #endif
    #if LIVE == true 
        #if PARTY == 0 || Party == all
            #define PROTOCOL_LIVE OECL0
            #include "oecl-P0_template.hpp"
        #endif
        #if PARTY == 1 || Party == all
            #define PROTOCOL_LIVE OECL1
            #include "oecl-P1_template.hpp"
        #endif
        #if PARTY == 2 || Party == all
            #define PROTOCOL_LIVE OECL2
            #include "oecl-P2_template.hpp"
        #endif
    #endif
    #if PROTOCOL == ttp 
        #define PROTOCOL_LIVE TTP
        #define PROTOCOL_INIT TTP_init
        #if INIT == true 
            #include "ttp_init_template.hpp"
        #endif
        #if LIVE == true
            #include "ttp.hpp"
        #endif
   #endif
#endif
