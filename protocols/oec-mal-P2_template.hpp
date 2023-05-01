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
#include "init_protocol_base.hpp"
#include "oec-mal_base.hpp"
#include "live_protocol_base.hpp"
//#include "oecl_base.hpp"
class OEC_MAL2
{
bool optimized_sharing;
public:
OEC_MAL2(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

OEC_MAL_Share public_val(DATATYPE a)
{
    return OEC_MAL_Share(a,SET_ALL_ZERO());
}

OEC_MAL_Share Not(OEC_MAL_Share a)
{
    return OEC_MAL_Share(NOT(a.v),a.r);
}

// Receive sharing of ~XOR(a,b) locally
OEC_MAL_Share Xor(OEC_MAL_Share a, OEC_MAL_Share b)
{
   return OEC_MAL_Share(XOR(a.v,b.v),XOR(a.r,b.r));
}



void prepare_and(OEC_MAL_Share a, OEC_MAL_Share b, OEC_MAL_Share &c)
{
DATATYPE cr = XOR(getRandomVal(P023),getRandomVal(P123));
/* DATATYPE r234 = getRandomVal(P123); */
DATATYPE r234 = getRandomVal(P3); //Probably sufficient to only generate with P3
/* c.r = getRandomVal(P3); */
DATATYPE o1 = receive_from_live(P0);
send_to_live(P1, XOR( XOR( XOR(o1,cr) , AND(a.r,b.r) ) , AND(a.v,b.v))); 
send_to_live(P0, XOR( XOR(r234,cr) , AND( XOR(a.v,a.r) ,XOR(b.v,b.r)))); 
c.v = XOR ( o1, XOR( AND(a.v,b.r) , AND(b.v,a.r)));
c.r = cr;
}

void complete_and(OEC_MAL_Share &c)
{
c.v = XOR(c.v, receive_from_live(P1)); 
}

void prepare_reveal_to_all(OEC_MAL_Share a)
{
}


DATATYPE complete_Reveal(OEC_MAL_Share a)
{
return XOR(a.v, receive_from_live(P3));
}

OEC_MAL_Share* alloc_Share(int l)
{
    return new OEC_MAL_Share[l];
}


void prepare_receive_from(OEC_MAL_Share a[], int id, int l)
{
if(id == PSELF)
{
for(int i = 0; i < l; i++)
{
    DATATYPE x_1 = getRandomVal(P023); // held by P1,P3 (this),P4
    DATATYPE x_2 = getRandomVal(P123); // held by P2,P3 (this),P4
    DATATYPE x_3 = XOR(x_1,x_2);
    a[i].r = x_3;
    a[i].v = get_input_live();
    /* a[i].p1 = getRandomVal(0); *1/ */
    send_to_live(P0, XOR(a[i].v,x_3));
    send_to_live(P1, XOR(a[i].v,x_3));
    a[i].v = XOR(a[i].v,x_2);
}
}
else if(id == P0)
{
    for(int i = 0; i < l; i++)
    {
        a[i].r = getRandomVal(P023);
    }
}
else if(id == P1)
{
    for(int i = 0; i < l; i++)
    {
        a[i].r = getRandomVal(P123);
    }
}
else if(id == P3)
{
    for(int i = 0; i < l; i++)
    {
    DATATYPE r023 = getRandomVal(P023);
    DATATYPE r123 = getRandomVal(P123);

        a[i].r = XOR(r023,r123);
    }
}

}

void complete_receive_from(OEC_MAL_Share a[], int id, int l)
{
if(id != PSELF)
{
    for(int i = 0; i < l; i++)
    {
        a[i].v = receive_from_live(id);
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
    communicate_live();
}

};
