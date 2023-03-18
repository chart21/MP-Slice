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
#include "sharemind_base.hpp"
#include "live_protocol_base.hpp"
//#define XOR_Share DATATYPE
#define SHARE DATATYPE

class TTP
{
bool input_srngs;
    public:
TTP(bool use_srngs) {input_srngs = use_srngs;}

DATATYPE share(DATATYPE a)
{
send_to_live(P2, a);
return a;
}




void share(DATATYPE a[], int length)
{
if(PARTY != 2)
{
for(int l = 0; l < length; l++)
    a[l] = share(a[l]);
}
}

XOR_Share public_val(DATATYPE a)
{
    return a;
}

DATATYPE Not(DATATYPE a)
{
   return NOT(a);
}

// Receive sharing of ~XOR(a,b) locally
DATATYPE Xor(DATATYPE a, DATATYPE b)
{
   return XOR(a, b);
}



void prepare_and(XOR_Share a, XOR_Share b, XOR_Share &c)
{
c = AND(a,b);
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(XOR_Share &c)
{
}

void prepare_reveal_to_all(DATATYPE a)
{
    if(PARTY == 2){
    for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
        send_to_live(t, a);
    }   
}



DATATYPE complete_Reveal(DATATYPE a)
{
DATATYPE result = a;
if(PARTY != 2)
{
    result = receive_from_live(P2);
}

return result;
}




void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == PSELF && PARTY != P2)
{
    for(int s = 0; s < l; s++)
    {
        share(get_input_live());
    }
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{

#if PARTY == 2
if(id == P2)
{
    for(int s = 0; s < l; s++)
    {
        a[s] = get_input_live();
    }
}
else
{
for (int i = 0; i < l; i++) {
    a[i] = receive_from_live(id);
}
}
#endif
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}



void finalize()
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
    communicate_live();
}

};
