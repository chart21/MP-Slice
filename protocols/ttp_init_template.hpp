#pragma once
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
#include "init_protocol_base.hpp"
#define INIT_SHARE DATATYPE

class TTP_init
{
bool input_srngs;
public:
TTP_init(bool use_srngs) {input_srngs = use_srngs;}




DATATYPE share_SRNG(DATATYPE a)
{
    return a;
}

XOR_Share receive_share_SRNG(int player)
{
XOR_Share dummy;
return dummy;
}

DATATYPE share(DATATYPE a)
{
#if player_id == 3
sending_args[2].elements_to_send[sending_args[2].send_rounds] += 1;
#else
sending_args[1].elements_to_send[sending_args[1].send_rounds] += 1;
#endif
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
   return a;
}

// Receive sharing of ~XOR(a,b) locally
DATATYPE Xor(DATATYPE a, DATATYPE b)
{
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(DATATYPE &a, DATATYPE &b)
{
//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE complete_and(DATATYPE a, DATATYPE b)
{
DATATYPE dummy;
return dummy;
}

void prepare_reveal_to_all(DATATYPE a)
{
    if(player_id == 2)
    {
        for(int t = 0; t < num_players-1; t++) 
        {
            sending_args[t].elements_to_send[sending_args[t].send_rounds] += 1;
        }//add to send buffer
    
    }
}    


// These functions need to be somewhere else

DATATYPE complete_Reveal(DATATYPE a)
{
if(player_id != 2)
{
#if PARTY == 3
    receiving_args[2].elements_to_rec[receiving_args[2].rec_rounds -1]+=1;
#else
    receiving_args[1].elements_to_rec[receiving_args[1].rec_rounds -1]+=1;
#endif
}
return a;
}

XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}

void receive_from_comm(DATATYPE a[], int id, int l)
{
if(id == player_id)
{
    return;
}
if(player_id == 2)
{
        for (int i = 0; i < l; i++) {
if (id == 3)
            receiving_args[2].elements_to_rec[receiving_args[2].rec_rounds -1] += 1;
else
            receiving_args[id].elements_to_rec[receiving_args[id].rec_rounds -1] += 1;
        }
    }
}

void receive_from(DATATYPE a[], int id, int l)
{
receive_from_comm(a, id, l);
}

void complete_receive_from_comm(DATATYPE a[], int id, int l)
{
receive_from_comm(a, id, l);
}


void prepare_receive_from_comm(DATATYPE a[], int id, int l)
{

if(id == player_id && player_id != 2)
{
    for(int i = 0; i < l; i++) 
#if PARTY == 3
sending_args[2].elements_to_send[sending_args[2].send_rounds] += 1;
#else
sending_args[1].elements_to_send[sending_args[1].send_rounds] += 1;
#endif
}
}

void prepare_receive_from(DATATYPE a[], int id, int l)
{
prepare_receive_from_comm(a, id, l);
}


void complete_receive_from(DATATYPE a[], int id, int l)
{
complete_receive_from_comm(a, id, l);
}

void send()
{
send_();
}
void receive()
{
    receive_();
}
void communicate()
{
communicate_();
}

void finalize(std::string* ips)
{
    finalize_(ips);
}


};
