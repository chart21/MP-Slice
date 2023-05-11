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
class OEC_MAL0_init
{
bool optimized_sharing;
public:
OEC_MAL0_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}


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
    store_compare_view_init(P2);
#else
#if PRE == 1
    pre_send_to_(P2);
#else
    send_to_(P2);
#endif
#endif
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(DATATYPE &c)
{
#if PROTOCOL == 10 || PROTOCOL == 12 || PROTOOCL == 8
#if PRE == 1
    pre_receive_from_(P3);
#else
receive_from_(P3);
#endif
receive_from_(P2);

store_compare_view_init(P1);
/* store_compare_view_init(P1); */
store_compare_view_init(P012);
# elif PROTOCOL == 11
receive_from_(P2);
receive_from_(P2); // receive ab from P2
store_compare_view_init(P1);
store_compare_view_init(P1);
store_compare_view_init(P3);
#endif
}

void prepare_reveal_to_all(DATATYPE a)
{
#if PRE == 0
    send_to_(P3);
#endif
}    



DATATYPE complete_Reveal(DATATYPE a)
{
#if PRE == 1
    pre_receive_from_(P3);
    send_to_(P3);
#else
receive_from_(P3);
#endif
    store_compare_view_init(P0123);
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
/* return; */
/* old: */

if(id == PSELF)
{
    for(int i = 0; i < l; i++)
    {
        send_to_(P1);
        send_to_(P2);
    }
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{
    if(id != PSELF)
    {
        for(int i = 0; i < l; i++)
            receive_from_(id);
        if(id != P1)
            for(int i = 0; i < l; i++)
                store_compare_view_init(P1);
        if(id != P2)
            for(int i = 0; i < l; i++)
                store_compare_view_init(P2);
        
    }
}





void send()
{
send_();
}

// P0 only has 1 receive round
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
