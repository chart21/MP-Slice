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



#if NEW_WAY == 1
void prepare_and(OEC_MAL_Share a, OEC_MAL_Share b, OEC_MAL_Share &c)
{
   c.r = XOR(getRandomVal(P023), getRandomVal(P123));
   /* DATATYPE r234 = getRandomVal(P123); */
   DATATYPE r234 =
       getRandomVal(P123); // Probably sufficient to only generate with P3 ->
                           // Probably not because of verification
/* c.r = getRandomVal(P3); */
#if PROTOCOL == 12
#if PRE == 1
   DATATYPE o1 = pre_receive_from_live(P3);
#else
   DATATYPE o1 = receive_from_live(P3);
#endif
   store_compare_view(P0, o1);
#else
   DATATYPE o1 = receive_from_live(P0);
   store_compare_view(P3, o1);
#endif
   DATATYPE m3_ex_cr = XOR(XOR(o1, AND(a.r, b.r)), AND(a.v, b.v));
   DATATYPE m3 = XOR(m3_ex_cr, c.r);
   send_to_live(P1, m3);


   c.v = AND(XOR(a.v, a.r), XOR(b.v, b.r));

#if PROTOCOL == 10 || PROTOCOL == 12
   DATATYPE m3_prime = XOR(XOR(r234, c.r), c.v);
   /* DATATYPE m3_prime = XOR(XOR(r234, c.r), AND(XOR(a.v, a.r), XOR(b.v, b.r))); */
   send_to_live(P0, m3_prime);
#endif
   c.v = XOR(c.v, m3_ex_cr);
   c.m = XOR(m3, r234);
}

void complete_and(OEC_MAL_Share &c)
{

DATATYPE m2 = receive_from_live(P1);
c.v = XOR(c.v, m2);
/* c.m = XOR(c.m, m2); */
/* DATATYPE cm = XOR(c.m, m2); */
#if PROTOCOL == 11
send_to_live(P0, XOR(c.m, m2)); // let P0 verify m_2 XOR m_3
send_to_live(P0, c.v); // let P0 obtain ab
#endif

#if PROTOCOL == 10 || PROTOCOL == 12
store_compare_view(P012, XOR(c.m, m2));
#endif
/* store_compare_view(P0, c.v); */
}


#else

void prepare_and(OEC_MAL_Share a, OEC_MAL_Share b, OEC_MAL_Share &c)
{
   c.r = XOR(getRandomVal(P023), getRandomVal(P123));
   /* DATATYPE r234 = getRandomVal(P123); */
   DATATYPE r234 =
       getRandomVal(P123); // Probably sufficient to only generate with P3 ->
                           // Probably not because of verification
/* c.r = getRandomVal(P3); */
#if PROTOCOL == 12
#if PRE == 1
   DATATYPE o1 = pre_receive_from_live(P3);
#else
   DATATYPE o1 = receive_from_live(P3);
#endif
   store_compare_view(P0, o1);
#else
   DATATYPE o1 = receive_from_live(P0);
   store_compare_view(P3, o1);
#endif
   DATATYPE m3 = XOR(XOR(XOR(o1, c.r), AND(a.r, b.r)), AND(a.v, b.v));
   send_to_live(P1, m3);

#if PROTOCOL == 10 || PROTOCOL == 12
   DATATYPE m3_prime = XOR(XOR(r234, c.r), AND(XOR(a.v, a.r), XOR(b.v, b.r)));
   send_to_live(P0, m3_prime);
#endif

   c.v = XOR(o1, XOR(AND(a.v, b.r), AND(b.v, a.r)));
   c.m = XOR(m3, r234);
}

void complete_and(OEC_MAL_Share &c)
{

DATATYPE m2 = receive_from_live(P1);
c.v = XOR(c.v, m2);

/* c.m = XOR(c.m, m2); */
/* DATATYPE cm = XOR(c.m, m2); */
#if PROTOCOL == 11
send_to_live(P0, XOR(c.m, m2)); // let P0 verify m_2 XOR m_3
send_to_live(P0, c.v); // let P0 obtain ab
#endif

#if PROTOCOL == 10 || PROTOCOL == 12
store_compare_view(P012, XOR(c.m, m2));
#endif
/* store_compare_view(P0, c.v); */
}

#endif

void prepare_reveal_to_all(OEC_MAL_Share a)
{
}


DATATYPE complete_Reveal(OEC_MAL_Share a)
{
#if PRE == 1
DATATYPE result = XOR(a.v, pre_receive_from_live(P3));
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
        if(id != P0)
            for(int i = 0; i < l; i++)
                store_compare_view(P0,a[i].v);
        if(id != P1)
            for(int i = 0; i < l; i++)
                store_compare_view(P1,XOR(a[i].v,a[i].r));
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
