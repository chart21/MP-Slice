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
#if PARTY == 3
sending_args[2].sent_elements[sending_rounds][sb] = a;
#else
sending_args[1].sent_elements[sending_rounds][sb] = a;
#endif
sb+=1;
return a;
}




void share(DATATYPE a[], int length)
{
if(player_id != 2)
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



void prepare_and(XOR_Share &a, XOR_Share &b)
{
}

// NAND both real Values to receive sharing of ~ (a&b) 
XOR_Share complete_and(XOR_Share a, XOR_Share b)
{
return AND(a,b);
}

void prepare_reveal_to_all(DATATYPE a)
{
    if(player_id == 2){
    for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
        sending_args[t].sent_elements[sending_rounds][sb] = a;
    sb += 1;
    }   
}



DATATYPE complete_Reveal(DATATYPE a)
{
DATATYPE result = a;
if(player_id != 2)
{
    #if PARTY == 3
    result = receiving_args[2].received_elements[rounds-1][rb];
    #else
    result = receiving_args[1].received_elements[rounds-1][rb];
    #endif
    rb+=1;
}

return result;
}




void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == player_id && player_id != 2)
{
    for(int s = 0; s < l; s++)
    {
        share(player_input[share_buffer[id]]);  
        share_buffer[id]+=1;
    }
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{

#if PARTY == 2
if(id == 2)
{
    for(int s = 0; s < l; s++)
    {
        a[s] = player_input[share_buffer[3]];
        share_buffer[3]+=1;
    }
}
else
{
for (int i = 0; i < l; i++) {
        if(id == 3)
        {a[i] = receiving_args[2].received_elements[rounds-1][share_buffer[2]];
        share_buffer[2]+=1;
        }
        else
        {
        a[i] = receiving_args[id].received_elements[rounds-1][share_buffer[id]];
        share_buffer[id]+=1;
        }
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
