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
class OEC_MAL3_init
{
bool optimized_sharing;
public:
OEC_MAL3_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}


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
store_compare_view_init(P2);
#if PROTOCOL == 10
#if PRE == 1
    pre_send_to_(P0);
#else
    send_to_(P0);
#endif
#elif PROTOCOL == 11
store_compare_view_init(P0);
#endif
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(DATATYPE &c)
{
}

void prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < 3; t++) 
    {
        #if PRE == 1 
    pre_send_to_(t);
#else
    send_to_(t);
#endif

    }//add to send buffer
}    



DATATYPE complete_Reveal(DATATYPE a)
{
receive_from_(P0);
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
        send_to_(P0);
        send_to_(P1);
        send_to_(P2);
    }
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{
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
