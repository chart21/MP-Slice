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
#define SHARE DATATYPE
class OEC2
{
bool optimized_sharing;
int prep_buffer;
public:
OEC2(bool optimized_sharing) {
this->optimized_sharing = optimized_sharing;
prep_buffer = 0;
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
   return XOR(a,b);
}



//prepare AND -> send real value a&b to other P
void prepare_and(DATATYPE &a, DATATYPE &b)
{
DATATYPE ry = getRandomVal(0);

#if PRE == 1
DATATYPE o1 = pre_receive_from_live(P0); //TODO different share_buffer for pres
DATATYPE o2 = pre_receive_from_live(P0);
#else
DATATYPE o1 = receive_from_live(P0);
DATATYPE o2 = receive_from_live(P0);
#endif
a = XOR(ry, AND(XOR(a,o1),XOR(b,o2)));
send_to_live(P1,a);
}


// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE complete_and(DATATYPE a, DATATYPE b)
{
b = receive_from_live(P1);
return XOR(a, b); 
}

void prepare_reveal_to_all(DATATYPE a)
{
    send_to_live(P0,a);
}    



DATATYPE complete_Reveal(DATATYPE a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
#if PRE == 1 && (OPT_SHARE == 0 || SHARE_PREP == 1) 
    a = XOR(a,pre_receive_from_live(P0));
#else 
    a = XOR(a,receive_from_live(P0));
#endif
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == 2)
{
for(int i = 0; i < l; i++)
{
    a[i] = get_input_live();
    a[i] = XOR(a[i],getRandomVal(0));
    send_to_live(P1,a[i]);
}
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{
if(id == player_id)
    return;
else if(id == 0)
{
    #if OPT_SHARE == 1
        for(int i = 0; i < l; i++)
            a[i] = SET_ALL_ZERO();
    #else
        for(int i = 0; i < l; i++)
        {
            #if PRE == 1 && SHARE_PREP == 1
            a[i] = pre_receive_from_live(P0);
            #else
            a[i] = receive_from_live(P0);
            #endif
        }
    #endif
}
else if(id == 1)
{
for(int i = 0; i < l; i++)
{
    a[i] = receive_from_live(P1);
}
}


/* for(int i = 0; i < l; i++) */
/* { */
/* a[i] = receiving_args[id].received_elements[rounds-1][share_buffer[id]]; */
/* share_buffer[id] +=1; */
/* } */
}



void finalize(std::string* ips)
{
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not receive from itself
    receiving_args[t].player_count = num_players;
    receiving_args[t].received_elements = new DATATYPE*[receiving_args[t].rec_rounds]; //every thread gets its own pointer array for receiving elements
    
    /* receiving_args[t].elements_to_rec = new int[total_comm]; */
    /* for (int i = 1 - use_srng_for_inputs; i < total_comm -1; i++) { */
    /* receiving_args[t].elements_to_rec[i] = elements_per_round[i]; */
    /* } */
    /* receiving_args[t].elements_to_rec[0] = 0; // input sharing with SRNG */ 
    /* if(use_srng_for_inputs == 0) */
    /*     receiving_args[t].elements_to_rec[0] = input_length[t+offset]; //input shares to receive from that player */
    /* receiving_args[t].elements_to_rec[total_comm-1] = reveal_length[player_id]; //number of revealed values to receive from other players */
    receiving_args[t].player_id = player_id;
    receiving_args[t].connected_to = t+offset;
    receiving_args[t].ip = ips[t];
    receiving_args[t].hostname = (char*)"hostname";
    receiving_args[t].port = (int) base_port + player_id * (num_players-1) + t; //e.g. P0 receives on base port from P1, P2 on base port + num_players from P0 6000,6002
    /* std::cout << "In main: creating thread " << t << "\n"; */
    //init_srng(t, (t+offset) + player_id); 
}
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not send to itself
    sending_args[t].sent_elements = new DATATYPE*[sending_args[t].send_rounds];
    /* sending_args[t].elements_to_send[0] = 0; //input sharing with SRNGs */ 
    sending_args[t].player_id = player_id;
    sending_args[t].player_count = num_players;
    sending_args[t].connected_to = t+offset;
    sending_args[t].port = (int) base_port + (t+offset) * (num_players -1) + player_id - 1 + offset; //e.g. P0 sends on base port + num_players  for P1, P2 on base port + num_players for P0 (6001,6000)
    /* std::cout << "In main: creating thread " << t << "\n"; */
    sending_args[t].sent_elements[0] = NEW(DATATYPE[sending_args[t].elements_to_send[0]]); // Allocate memory for first round
       
}
rounds = 0;
sending_rounds = 0;
rb = 0;
sb = 0;
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
