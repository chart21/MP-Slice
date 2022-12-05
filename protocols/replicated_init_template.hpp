#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"
#include "replicated_base.hpp"

class Replicated_init{
bool input_srngs;
    public:
Replicated_init(bool use_srngs) {input_srngs = use_srngs;}
Share share_SRNG(DATATYPE a)
{

sending_args[pprev].elements_to_send[sending_args[pprev].send_rounds] +=1;
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
Share dummy;
return dummy;
}

Share receive_share_SRNG(int player)
{
receiving_args[player].elements_to_rec[receiving_args[player].rec_rounds - 1] +=1;
Share s;
return s;
}



void share(Share a[], int length)
{

    for(int l = 0; l < length; l++)
    {
    share_SRNG(player_input[share_buffer[player_id]]);  
}
}


void prepare_receive_from(Share a[], int id, int l)
{
    if(id == player_id)
        share(a,l);
}

void complete_receive_from(Share a[], int id, int l)
{
if(id == player_id)
    return;
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    a[i] = receive_share_SRNG(id - offset);

}
}


// Receive sharing of ~XOR(a,b) locally
Share Xor(Share a, Share b)
{
    return a; 
}

Share Xor_pub(Share a, DATATYPE b)
{
    return a; 
}

Share public_val(DATATYPE a)
{
    Share dummy;
    return dummy; 
}

Share Not(Share a)
{
    return a; 
}


//prepare AND -> send real value a&b to other P
void prepare_and(Share &a, Share &b)
{
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share complete_and(Share a, Share b)
{
receiving_args[pprev].elements_to_rec[receiving_args[pprev].rec_rounds - 1] +=1;
return a;
}

void prepare_reveal_to_all(Share a)
{
    sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
    //add to send buffer
}    



DATATYPE complete_Reveal(Share a)
{
DATATYPE result;
receiving_args[pprev].elements_to_rec[receiving_args[pprev].rec_rounds - 1] +=1;
return result;
}

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
    /* sending_args[t].total_rounds += 1; */
    /* sending_args[t].send_rounds += 1; */
    /* /1* receiving_args[t].total_rounds += 1; *1/ */
    /* receiving_args[t].rec_rounds +=1; */
    sending_args[t].total_rounds += 1;
    sending_args[t].send_rounds += 1;
    receiving_args[t].total_rounds += 1;
    receiving_args[t].rec_rounds +=1;
}
}
void communicate()
{
    send_and_receive();
}

Share* alloc_Share(int l)
{
    return new Share[l];
}

void receive_from_SRNG(Share a[], int id, int l)
{
if(id == player_id)
{
for (int i = 0; i < l; i++) {
  a[i] = share_SRNG(player_input[share_buffer[id]]);  
}
}
else{
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    a[i] = receive_share_SRNG(id - offset);
}
}
}
void receive_from(Share a[], int id, int l)
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

void finalize(char** ips)
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
void init(){
    for(int t=0;t<(num_players-1);t++) {
sending_args[t].elements_to_send[0] = 0;
receiving_args[t].elements_to_rec[0] = 0;
}
}
};
