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
//#include "oecl_base.hpp"
#define SHARE DATATYPE
class OECL2
{
bool optimized_sharing;
public:
OECL2(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

XOR_Share public_val(DATATYPE a)
{
    return a;
}

XOR_Share Not(XOR_Share a)
{
    return NOT(a);
}

// Receive sharing of ~XOR(a,b) locally
XOR_Share Xor(XOR_Share a, XOR_Share b)
{
   return XOR(a,b);
}



void prepare_and(XOR_Share &a, XOR_Share &b)
{

XOR_Share ap1 = getRandomVal(0); // P2 mask for P1
a = XOR(receiving_args[0].received_elements[rounds-1][share_buffer[0]], AND(a,b)); // P0_message + (a+rr) (b+rl)
sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(ap1,a); // + P2 mask
share_buffer[0]+=1;
send_count[1]+=1;
}

XOR_Share complete_and(XOR_Share a, XOR_Share b)
{
a = XOR(a, receiving_args[1].received_elements[rounds-1][share_buffer[1]]);
share_buffer[1]+=1;

return a; 
}

void prepare_reveal_to_all(XOR_Share a)
{
sending_args[0].sent_elements[sending_rounds][send_count[0]] = a;
send_count[0] += 1;
}    



DATATYPE complete_Reveal(XOR_Share a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
a = XOR(a,receiving_args[0].received_elements[rounds-1][share_buffer[0]]); 
share_buffer[0]+=1;
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new XOR_Share[l];
}


void prepare_receive_from(XOR_Share a[], int id, int l)
{
if(id == 2)
{
for(int i = 0; i < l; i++)
{
    a[i] = player_input[share_buffer[2]];
    /* a[i].p1 = getRandomVal(0); *1/ */
    share_buffer[2] += 1;
    sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(getRandomVal(0),a[i]);
    send_count[1]+=1;
}
}
}

void complete_receive_from(XOR_Share a[], int id, int l)
{
if(id == 0)
{
    for(int i = 0; i < l; i++)
    {
        a[i] = receiving_args[0].received_elements[rounds-1][share_buffer[0]];
        share_buffer[0] +=1;
    }
}
else if(id == 1)
{
for(int i = 0; i < l; i++)
{
/* a[i].p1 = SET_ALL_ZERO(); */
a[i] = receiving_args[1].received_elements[rounds-1][share_buffer[1]];
share_buffer[1] +=1;
/* a[i].p1 = SET_ALL_ZERO(); */
}
}

/* int offset = {id > player_id ? 1 : 0}; */
/* int player = id - offset; */
/* for(int i = 0; i < l; i++) */
/* { */
/* a[i] = receiving_args[player].received_elements[rounds-1][share_buffer[player]]; */
/* share_buffer[player] +=1; */
/* } */
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
