#pragma once
#include <chrono>
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
//#define pnext player_id == 0 ? 0 : player_id == 1 ? 1 : 0 
/* #define pnext (player_id + 1)%3 */
//#define pprev player_id == 0 ? 1 : player_id == 1 ? 0 : 1 
/* #define pprev (player_id - 1)%3 */
// XOR_Share of each player is ~a

#include "../utils/randomizer.h"
//#include "sharemind_base.hpp"
#include "oecl_base.hpp"
#include "live_protocol_base.hpp"
#define PRE_SHARE OECL_Share
class OECL0
{
bool optimized_sharing;
public:
OECL0(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

OECL_Share public_val(DATATYPE a)
{
    return OECL_Share(SET_ALL_ZERO(),SET_ALL_ZERO());
}

OECL_Share Not(OECL_Share a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
OECL_Share Xor(OECL_Share a, OECL_Share b)
{
    a.p1 = XOR(a.p1,b.p1);
    a.p2 = XOR(a.p2,b.p2);
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(OECL_Share a, OECL_Share b, OECL_Share &c)
{
/* DATATYPE rl = getRandomVal(0); */
/* DATATYPE rr = getRandomVal(0); */

/* DATATYPE rx = getRandomVal(0); */
/* DATATYPE ry = getRandomVal(1); */
/* DATATYPE maskP1 = XOR(a.p1,b.p1); */
/* DATATYPE maskP2 = XOR(a.p2,b.p2); */
DATATYPE r124 = SET_ALL_ZERO();
DATATYPE o1 = XOR( AND(a.p2,b.p2), r124);

#if PRE == 1
pre_send_to_live(P2, o1);
#else
send_to_live(P2, o1);
#endif
c.p1 = XOR( AND(a.p1,b.p2), AND(b.p2,a.p1));
}

void complete_and(OECL_Share &c)
{
#if PRE == 1
//Only receive from P3
#else
c.p1 = XOR(c.p1, XOR( receive_from_live(P3), receive_from_live(P2)));
#endif
}


void prepare_reveal_to_all(OECL_Share a)
{
}    



DATATYPE complete_Reveal(OECL_Share a)
{
#if PRE == 1
#else
return XOR(a.p1, receive_from_live(P3));
#endif
}


OECL_Share* alloc_Share(int l)
{
    return new OECL_Share[l];
}



void prepare_receive_from(OECL_Share a[], int id, int l)
{
if(id == PSELF)
{
    for(int i = 0; i < l; i++)
    {
    a[i].p1 = get_input_live();
    a[i].p2 = SET_ALL_ZERO();
    send_to_live(P1, a[i].p2);
    send_to_live(P2, a[i].p2);
    } 
}
}
void complete_receive_from(OECL_Share a[], int id, int l)
{
if(id != PSELF)
{
    for(int i = 0; i < l; i++)
    {
    a[i].p1 = receive_from_live(id);
    a[i].p2 = SET_ALL_ZERO();
    }
    
}
}




void send()
{
    send_live();
}

void receive()
{
    receive_live();
}

void communicate()
{
#if PRE == 0
    communicate_live();
#endif
}

};
