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
#include "init_protocol_base.hpp"
#include "oec-mal_base.hpp"
#include "live_protocol_base.hpp"
#define PRE_SHARE OEC_MAL_Share
class OEC_MAL0
{
bool optimized_sharing;
public:
OEC_MAL0(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

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
    a.v = XOR(a.v,b.v);
    a.r = XOR(a.r,b.r);
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(OEC_MAL_Share a, OEC_MAL_Share b, OEC_MAL_Share &c)
{
DATATYPE cr = XOR(getRandomVal(P013),getRandomVal(P023));
DATATYPE r124 = getRandomVal(P013);
DATATYPE o1 = XOR( AND(a.r,b.r), r124);
#if MAL == 1
/* store_compare_view(P3, o1); */
store_compare_view(P3,SET_ALL_ONE());
#endif
#if PRE == 1
pre_send_to_live(P2, o1);
#else
send_to_live(P2, o1);
#endif
c.v = XOR( AND(a.v,b.r), AND(b.v,a.r));
c.r = cr;
/* c.r = getRandomVal(P3); */

}

void complete_and(OEC_MAL_Share &c)
{
#if PRE == 1
DATATYPE o_4 = pre_receive_from_live(P3);
#else
DATATYPE o_4 = receive_from_live(P3);
#endif
DATATYPE m3_prime = receive_from_live(P2);
c.v = XOR(c.v, XOR( m3_prime, o_4));
}



void prepare_reveal_to_all(OEC_MAL_Share a)
{
    send_to_live(P3, a.v);
}    



DATATYPE complete_Reveal(OEC_MAL_Share a)
{
#if PRE == 1
return XOR(a.v, pre_receive_from_live(P3));
#else
return XOR(a.v, receive_from_live(P3));
#endif
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
    a[i].v = get_input_live();
    DATATYPE x_2 = getRandomVal(P013);
    DATATYPE x_3 = getRandomVal(P023);
    a[i].r = XOR(x_2, x_3);
    
    send_to_live(P1,XOR(a[i].v, x_3));
    send_to_live(P2,XOR( a[i].v, x_2));
    a[i].v = XOR(a[i].v,x_3);
    } 
}
else if(id == P1)
{
    for(int i = 0; i < l; i++)
    {
    a[i].r = getRandomVal(P013);
    }
}
else if(id == P2)
{
    for(int i = 0; i < l; i++)
    {
    a[i].r = getRandomVal(P023);
    }
}
else if(id == P3)
{
    for(int i = 0; i < l; i++)
    {
    DATATYPE r013 = getRandomVal(P013);
    DATATYPE r023 = getRandomVal(P023);
    a[i].r = XOR(r013, r023);
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
#if PRE == 0
    communicate_live();
#endif
}

};
