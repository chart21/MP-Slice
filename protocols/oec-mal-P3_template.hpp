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
#include "oec-mal_base.hpp"
#include "live_protocol_base.hpp"
#define PRE_SHARE OECL_Share
class OEC_MAL3
{
bool optimized_sharing;
public:
OEC_MAL3(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

Dealer_Share public_val(DATATYPE a)
{
    return Dealer_Share(SET_ALL_ZERO(),SET_ALL_ZERO(),SET_ALL_ZERO());
}

Dealer_Share Not(Dealer_Share a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
Dealer_Share Xor(Dealer_Share a, Dealer_Share b)
{
   return Dealer_Share(XOR(a.r0,b.r0),XOR(a.r1,b.r1),XOR(a.r2,b.r2));
}



//prepare AND -> send real value a&b to other P
void prepare_and(Dealer_Share a, Dealer_Share b, Dealer_Share &c)
{
c.r0 = getRandomVal(P0);
c.r1 = getRandomVal(P1);
c.r2 = getRandomVal(P2);
DATATYPE r124 = getRandomVal(P013); // used for verification
DATATYPE r234 = getRandomVal(P123);
DATATYPE o4 = XOR( XOR( AND(a.r1,b.r1) ,AND(a.r2,b.r2)),r234);

#if PRE == 1
pre_send_to_live(P0, o4);
#else
send_to_live(P0, o4);
#endif
}

void complete_and(Dealer_Share &c)
{
}


void prepare_reveal_to_all(Dealer_Share a)
{
#if PRE == 1
    pre_send_to_live(P0, a.r2);
    pre_send_to_live(P1, a.r2);
    pre_send_to_live(P2, a.r1);
#else
    send_to_live(P0, a.r2);
    send_to_live(P1, a.r2);
    send_to_live(P2, a.r1);
#endif
}    



DATATYPE complete_Reveal(Dealer_Share a)
{
#if PRE == 1
return XOR(a.r0, pre_receive_from_live(P0));
#else
return XOR(a.r0, receive_from_live(P0));
#endif
}


Dealer_Share* alloc_Share(int l)
{
    return new Dealer_Share[l];
}



void prepare_receive_from(Dealer_Share a[], int id, int l)
{
if(id == PSELF)
{
    for(int i = 0; i < l; i++)
    {
    DATATYPE v = get_input_live();
    DATATYPE r013 = getRandomVal(P013);
    DATATYPE r023 = getRandomVal(P023);
    DATATYPE r123 = getRandomVal(P123);

    a[i].r0 = XOR(r013,r023);
    a[i].r1 = XOR(r013,r123);
    a[i].r2 = XOR(r023,r123);
    send_to_live(P0, XOR(a[i].r2,v));
    send_to_live(P1, XOR(a[i].r2,v));
    send_to_live(P2, XOR(a[i].r1,v));
    } 
}
else if(id == P0)
{
    for(int i = 0; i < l; i++)
    {
    a[i].r1 = getRandomVal(P013);
    a[i].r2 = getRandomVal(P023);
    a[i].r0 = XOR(a[i].r1,a[i].r2);

    }
}
else if(id == P1)
{
    for(int i = 0; i < l; i++)
    {
    a[i].r0 = getRandomVal(P013);
    a[i].r2 = getRandomVal(P123);
    a[i].r1 = XOR(a[i].r0,a[i].r2);
    }
}
else if(id == P2)
{
    for(int i = 0; i < l; i++)
    {
    a[i].r0 = getRandomVal(P023);
    a[i].r1 = getRandomVal(P123);
    a[i].r2 = XOR(a[i].r0,a[i].r1);
    }
}
}

void complete_receive_from(Dealer_Share a[], int id, int l)
{
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
