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
class OECL1
{
bool optimized_sharing;
public:
OECL1(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

OECL_Share public_val(DATATYPE a)
{
    return OECL_Share(a,SET_ALL_ZERO());
}

OECL_Share Not(OECL_Share a)
{
    a.p1 = NOT(a.p1);
   return a;
}

// Receive sharing of ~XOR(a,b) locally
OECL_Share Xor(OECL_Share a, OECL_Share b)
{
   return OECL_Share(XOR(a.p1,b.p1),XOR(a.p2,b.p2));
}



//prepare AND -> send real value a&b to other P
void prepare_and(OECL_Share a, OECL_Share b , OECL_Share &c)
{
c.p1 = XOR(getRandomVal(P0), XOR(AND(a.p1,b.p2), AND(b.p1,a.p2))); //remove P1_mask, then (a+ra)rl + (b+rb)rr 
c.p2 = getRandomVal(P0); //generate P1_2 mask
send_to_live(P2,XOR(c.p1,c.p2)); 
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(OECL_Share &c)
{
c.p1 = XOR(c.p1, receive_from_live(P2));
}

void prepare_reveal_to_all(OECL_Share a)
{
return;
}    



DATATYPE complete_Reveal(OECL_Share a)
{
return XOR(a.p1, receive_from_live(P0));
}


OECL_Share* alloc_Share(int l)
{
    return new OECL_Share[l];
}


void prepare_receive_from(OECL_Share a[], int id, int l)
{
if(id == P0)
{
    for(int i = 0; i < l; i++)
    {
        a[i].p2 = getRandomVal(P0);
    }
}
else if(id == P1)
{
for(int i = 0; i < l; i++)
{
    a[i].p1 = get_input_live();
    a[i].p2 = getRandomVal(P0);
    send_to_live(P2,XOR(a[i].p1,a[i].p2));
}
}
}

void complete_receive_from(OECL_Share a[], int id, int l)
{
if(id == P0)
{
if(optimized_sharing == true)
{
    for(int i = 0; i < l; i++)
    {
        a[i].p1 = SET_ALL_ZERO(); 
    }
}
else{
    for(int i = 0; i < l; i++)
    {
        a[i].p1 = receive_from_live(P0);
    }
    
}
}
else if(id == P2)
{
for(int i = 0; i < l; i++)
{
a[i].p1 = receive_from_live(P2);
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
    communicate_live();
}

};
