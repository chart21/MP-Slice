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
DATATYPE r124 = getRandomVal(P013);
c.r = getRandomVal(P3);
send_to_live(P2,XOR(   XOR(AND(a.r,b.r), AND(a.v,b.v))  , XOR(c.r,r124))); 

c.v = XOR( XOR(      AND(a.v,b.r) , AND(b.v,a.r) ) , XOR(AND(a.r,b.r),r124));
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(OEC_MAL_Share &c)
{
c.v = XOR(c.v, receive_from_live(P2));
}

void prepare_reveal_to_all(OEC_MAL_Share a)
{
return;
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
