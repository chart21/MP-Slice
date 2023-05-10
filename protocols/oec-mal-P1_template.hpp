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
class OEC_MAL1
{
bool optimized_sharing;
public:
OEC_MAL1(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

OEC_MAL_Share public_val(DATATYPE a)
{
    return OEC_MAL_Share(a,SET_ALL_ZERO());
}

OEC_MAL_Share Not(OEC_MAL_Share a)
{
   a.v = NOT(a.v);
   return a;
}

// Receive sharing of ~XOR(a,b) locally
OEC_MAL_Share Xor(OEC_MAL_Share a, OEC_MAL_Share b)
{
   return OEC_MAL_Share(XOR(a.v,b.v),XOR(a.r,b.r));
}



//prepare AND -> send real value a&b to other P
void prepare_and(OEC_MAL_Share a, OEC_MAL_Share b , OEC_MAL_Share &c)
{
DATATYPE cr = XOR(getRandomVal(P013),getRandomVal(P123));
DATATYPE r124 = getRandomVal(P013);
DATATYPE r234 = getRandomVal(P123); //used for veryfying m3' sent by P3 -> probably not needed -> for verification needed
/* c.r = getRandomVal(P3); */
DATATYPE m_2 = XOR(   XOR(AND(a.r,b.r), AND(a.v,b.v))  , XOR(cr,r124));  
send_to_live(P2,m_2); 

/* DATATYPE m3_prime = XOR( XOR(r234,cr) , AND( XOR(a.v,a.r) ,XOR(b.v,b.r))); //computationally wise more efficient to verify ab instead of m_3 prime */

/* store_compare_view(P0,m3_prime); */

c.v = XOR( XOR(      AND(a.v,b.r) , AND(b.v,a.r) ) , r124);
c.r = cr;
c.m = XOR(m_2,r234);
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(OEC_MAL_Share &c)
{
DATATYPE m_3 = receive_from_live(P2);
c.v = XOR(c.v, m_3);

c.m = XOR(c.m,m_3);

#if PROTOCOL == 10
store_compare_view(P012,c.m);
#elif PROTOCOL == 11
store_compare_view(P0,c.m);
#endif
store_compare_view(P0,c.v);
}

void prepare_reveal_to_all(OEC_MAL_Share a)
{
return;
}    



DATATYPE complete_Reveal(OEC_MAL_Share a)
{
DATATYPE result = XOR(a.v, receive_from_live(P3));
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
    DATATYPE x_1 = getRandomVal(P013);
    DATATYPE x_3 = getRandomVal(P123);
    DATATYPE x_2 = XOR(x_1,x_3);
    a[i].r = x_2;
    send_to_live(P0,XOR(a[i].v,x_3));
    send_to_live(P2,XOR(a[i].v,x_2));
    a[i].v = XOR(a[i].v,x_3);
}
}
else if(id == P0)
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
    a[i].r = getRandomVal(P123);
    
  
}
}
else if(id == P3)
{
for(int i = 0; i < l; i++)
{
    DATATYPE r013 = getRandomVal(P013);
    DATATYPE r123 = getRandomVal(P123);

    a[i].r = XOR(r013,r123);
    
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
    communicate_live();
}

};
