#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
//#define pnext player_id == 0 ? 0 : player_id == 1 ? 1 : 0 
/* #define pnext (player_id + 1)%3 */
//#define pprev player_id == 0 ? 1 : player_id == 1 ? 0 : 1 
/* #define pprev (player_id - 1)%3 */
// XOR_Share of each player is ~a

#include "../utils/randomizer.h"
#include "sharemind_base.hpp"
#include "init_protocol_base.hpp"
class OEC_MAL2_init
{
bool optimized_sharing;
public:
OEC_MAL2_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}



XOR_Share public_val(DATATYPE a)
{
    return a;
}

DATATYPE Not(DATATYPE a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
DATATYPE Xor(DATATYPE a, DATATYPE b)
{
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(DATATYPE a, DATATYPE b, DATATYPE &c)
{
#if PROTOCOL == 12 || PROTOCOL == 8
store_compare_view_init(P0);
#if PRE == 1
pre_receive_from_(P3);
#else
receive_from_(P3);
#endif
#else
store_compare_view_init(P3);
#if PRE == 1
pre_receive_from_(P0);
#else
/* receive_from_(P0); */
#endif
#endif
send_to_(P1);
#if PROTOCOL == 10 || PROTOCOL == 12
send_to_(P0);
#endif

//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(DATATYPE &c)
{
    receive_from_(P1);
#if PROTOCOL == 11
send_to_(P0); // let P0 obtain ab
send_to_(P0); // let P0 verify m_2 XOR m_3
#endif

#if PROTOCOL == 10 || PROTOCOL == 12 || PROTOCOL == 8
store_compare_view_init(P012);
#endif

#if PROTOCOL == 8
 send_to_(P0); // let P0 obtain ab
#endif
}

void prepare_reveal_to_all(DATATYPE a)
{
}    



DATATYPE complete_Reveal(DATATYPE a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
#if PRE == 1 
    pre_receive_from_(P3);
#else
    receive_from_(P3);
#endif
#if PROTOCOL == 8
    store_compare_view_init(P0);
#else
    store_compare_view_init(P0123);
#endif
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == PSELF)
{
    for(int i = 0; i < l; i++)
    {
        send_to_(P0);
        send_to_(P1);
    }

}
}


void complete_receive_from(DATATYPE a[], int id, int l)
{
    if(id != PSELF)
    {
        for(int i = 0; i < l; i++)
            receive_from_(id);
        if(id != P0)
            for(int i = 0; i < l; i++)
                store_compare_view_init(P0);
        if(id != P1)
            for(int i = 0; i < l; i++)
                store_compare_view_init(P1);
    }
}





void send()
{
send_();
}
void receive()
{
    receive_();
}
void communicate()
{
communicate_();
}

void finalize(std::string* ips)
{
    finalize_(ips);
}

void finalize(std::string* ips, receiver_args* ra, sender_args* sa)
{
    finalize_(ips, ra, sa);
}


};
