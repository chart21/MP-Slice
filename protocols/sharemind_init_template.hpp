#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
//#define pnext player_id == 0 ? 0 : player_id == 1 ? 1 : 0 
/* #define pnext (player_id + 1)%3 */
//#define pprev player_id == 0 ? 1 : player_id == 1 ? 0 : 1 
/* #define pprev (player_id - 1)%3 */
// XOR_Share of each player is ~a
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"
#include "sharemind_base.hpp"
class Sharemind_init
{
bool input_srngs;
public:
Sharemind_init(bool use_srngs) {input_srngs = use_srngs;}
void send_and_receive()
{
for (int t = 0; t < num_players-1; t++)
{
/* if(sending_args[t].elements_to_send[sending_args[t].send_rounds] != 0) */
/* { */
/*     sending_args[t].total_rounds += 1; */
/*     sending_args[t].send_rounds += 1; */
/* } */
/* if(receiving_args[t].elements_to_rec[receiving_args[t].rec_rounds] != 0) */
/* { */
/*     receiving_args[t].total_rounds += 1; */
/*     receiving_args[t].rec_rounds +=1; */
        
/* } */

    sending_args[t].total_rounds += 1;
    sending_args[t].send_rounds += 1;
    receiving_args[t].total_rounds += 1;
    receiving_args[t].rec_rounds +=1;
}
}
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
sending_args[pprev].elements_to_send[sending_args[pprev].send_rounds] += 1;
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
/* DATATYPE dummy; */
/* return dummy; */
return a;
}


void share(Bit_Array a, int length)
{
if(input_srngs == true)
{
    return;
}
else{
    for(int l = 0; l < length; l++)
        a[l] = share(a[l]);
}                                      //
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



void reshare(DATATYPE a, DATATYPE u[])
{
 
}
//prepare AND -> send real value a&b to other P
void prepare_and(DATATYPE a, DATATYPE b)
{
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] += 1;
sending_args[pprev].elements_to_send[sending_args[pprev].send_rounds] += 1;
//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE complete_and(DATATYPE a, DATATYPE b)
{
/* receiving_args[pprev].elements_to_rec[rounds-1] += 1; */
/* receiving_args[pnext].elements_to_rec[rounds-1] += 1; */
receiving_args[pprev].elements_to_rec[receiving_args[pprev].rec_rounds -1] += 1;
receiving_args[pnext].elements_to_rec[receiving_args[pnext].rec_rounds -1] += 1;
DATATYPE dummy;
return dummy;
}

void prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < num_players-1; t++) 
    {
        sending_args[t].elements_to_send[sending_args[t].send_rounds] += 1;
        /* receiving_args[t].elements_to_rec[receiving_args[t].rec_rounds -1]+=1; */
    }//add to send buffer
}    


//reveal to specific player
void prepare_reveal_to(DATATYPE a, int id)
{
    if(player_id != id)
    {
    int offset = {player_id > id ? 1 : 0};
        sending_args[id - offset].sent_elements[sending_rounds][reveal_buffer[id]] = a;
    reveal_buffer[id] += 1;
    //add to send buffer
}
}
// These functions need to be somewhere else

DATATYPE complete_Reveal(DATATYPE a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
    for(int t = 0; t < num_players-1; t++) 
    {
        receiving_args[t].elements_to_rec[receiving_args[t].rec_rounds -1]+=1;
    }
return a;
}

void communicate()
{
    send_and_receive();
}

XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}

void receive_from_SRNG(Bit_Array a, int id, int l)
{
}
void receive_from_comm(Bit_Array a, int id, int l)
{
if(id == player_id)
{
    return;
}
else{
int offset = {id > player_id ? 1 : 0};
int player = id - offset;
for (int i = 0; i < l; i++) {
receiving_args[player].elements_to_rec[receiving_args[player].rec_rounds -1] += 1;
}
}
}


void receive_from(Bit_Array a, int id, int l)
{
if(input_srngs == true)
{
    receive_from_SRNG(a, id, l);
}
else
{
receive_from_comm(a, id, l);
}
}

void complete_receive_from_comm(Bit_Array a, int id, int l)
{
if(id == player_id)
    return;
else{
int offset = {id > player_id ? 1 : 0};
int player = id - offset;
for (int i = 0; i < l; i++) {
    receiving_args[player].elements_to_rec[receiving_args[player].rec_rounds -1] += 1;
}
}
}


void prepare_receive_from_comm(Bit_Array a, int id, int l)
{
if(id == player_id)
{
for (int i = 0; i < l; i++) {
    sending_args[pprev].elements_to_send[sending_args[pprev].send_rounds] += 1;
    sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
    }
}
else {

return;
    }
}

void prepare_receive_from(Bit_Array a, int id, int l)
{
if(input_srngs == true)
{
    return; //no sending needed
}
else
{
prepare_receive_from_comm(a, id, l);
}
}


void complete_receive_from(Bit_Array a, int id, int l)
{
if(input_srngs == true)
{
    return;
}
else
{
complete_receive_from_comm(a, id, l);
}
}



void finalize(char** ips)
{
for(int t=0;t<(num_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not receive from itself
    receiving_args[t].player_count = num_players;
    receiving_args[t].received_elements = new Bit_Array*[receiving_args[t].rec_rounds]; //every thread gets its own pointer array for receiving elements
    
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
    /* sending_args[t].elements_to_send[0] = 0; //input sharing with SRNGs */ 
    sending_args[t].sent_elements = new Bit_Array*[sending_args[t].send_rounds];
    sending_args[t].player_id = player_id;
    sending_args[t].player_count = num_players;
    sending_args[t].connected_to = t+offset;
    sending_args[t].port = (int) base_port + (t+offset) * (num_players -1) + player_id - 1 + offset; //e.g. P0 sends on base port + num_players  for P1, P2 on base port + num_players for P0 (6001,6000)
    /* std::cout << "In main: creating thread " << t << "\n"; */
    sending_args[t].sent_elements[0] = new Bit_Array(sending_args[t].elements_to_send[0]); // Allocate memory for first round
       
}
rounds = 0;
sending_rounds = 0;
rb = 0;
sb = 0;
}
};
