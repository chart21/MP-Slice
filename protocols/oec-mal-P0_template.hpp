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
c.r = XOR(getRandomVal(P013),getRandomVal(P023));
/* DATATYPE r124 = getRandomVal(P013); */
DATATYPE x1y1 = AND(a.r, b.r);
/* DATATYPE o1 = XOR( x1y1, r124); */
DATATYPE o1 = XOR( x1y1, getRandomVal(P013));
c.v = XOR( AND(a.v,b.r), AND(b.v,a.r));
/* DATATYPE m3_flat = AND(a.v,b.v); */
c.m = XOR(x1y1, XOR( XOR(AND(a.v,b.v), AND( XOR(a.v, a.r), XOR(b.v, b.r))), c.r));
#if PROTOCOL == 12
store_compare_view(P2,o1);
#else
    #if PRE == 1
        pre_send_to_live(P2, o1);
    #else
        /* send_to_live(P2, o1); */
    #endif
#endif

}

void complete_and(OEC_MAL_Share &c)
{
#if PROTOCOL == 10 || PROTOCOL == 12
#if PRE == 1
DATATYPE o_4 = pre_receive_from_live(P3);
#else
DATATYPE o_4 = receive_from_live(P3);
#endif
#elif PROTOCOL == 11
DATATYPE m_2XORm_3 = receive_from_live(P2);
c.v = XOR(receive_from_live(P2),c.r); // receive ab + r_2 from P2 (P3 in paper), need to convert to ab + r_3
store_compare_view(P1, m_2XORm_3); // Verify if P_2 sent correct message m_2 XOR m_3
store_compare_view(P1, c.v); // Verify if P_2 sent correct message of ab
store_compare_view(P3, XOR(m_2XORm_3,c.m)); // x2y2 + x3y3 + r234 should remain
#endif

#if PROTOCOL == 10 || PROTOCOL == 12
/* DATATYPE m3_prime = receive_from_live(P2); */
c.v = XOR(c.v, XOR( receive_from_live(P2), o_4));

/* c.m = XOR(c.m, o_4); */
store_compare_view(P012,XOR(c.m, o_4));
store_compare_view(P1, c.v); // to verify m_3 prime
#endif
}



void prepare_reveal_to_all(OEC_MAL_Share a)
{
#if PRE == 0
    send_to_live(P3, a.v);
#endif
}    



DATATYPE complete_Reveal(OEC_MAL_Share a)
{
#if PRE == 1
DATATYPE result = XOR(a.v, pre_receive_from_live(P3));
send_to_live(P3, result);
#else
DATATYPE result = XOR(a.v, receive_from_live(P3));
#endif
store_compare_view(P0123, result);
return result;
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

        if(id != P1)
            for(int i = 0; i < l; i++)
                store_compare_view(P1,a[i].v);
        if(id != P2)
            for(int i = 0; i < l; i++)
                store_compare_view(P2,XOR(a[i].v,a[i].r));

    
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
/* #if PRE == 0 */
    communicate_live();
/* #endif */
}

};
