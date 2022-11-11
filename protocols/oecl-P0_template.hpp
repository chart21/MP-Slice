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
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"
//#include "sharemind_base.hpp"
#include "oecl_base.hpp"
class OECL0
{
bool optimized_sharing;
public:
OECL0(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

OECL_Share public_val(DATATYPE a)
{
    return OECL_Share(SET_ALL_ZERO(),SET_ALL_ZERO());
}

OECL_Share Not(OECL_Share a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
OECL_Share Xor(OECL_Share a, OECL_Share b)
{
    a.p1 = XOR(a.p1,b.p1);
    a.p2 = XOR(a.p2,b.p2);
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(OECL_Share &a, OECL_Share &b)
{
/* DATATYPE rl = getRandomVal(0); */
/* DATATYPE rr = getRandomVal(0); */

/* DATATYPE rx = getRandomVal(0); */
/* DATATYPE ry = getRandomVal(1); */
/* DATATYPE maskP1 = XOR(a.p1,b.p1); */
/* DATATYPE maskP2 = XOR(a.p2,b.p2); */

DATATYPE maskP1 = getRandomVal(0);
DATATYPE maskP1_2 = getRandomVal(0);
DATATYPE maskP2 = getRandomVal(1);

sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(maskP1,  XOR( XOR( AND(a.p1,b.p2) , AND(a.p2,b.p1) ) ,  AND(a.p2,b.p2) ) ); 
// maskP1 + ra rl + rr rb + rr rl
/* sending_args[1].sent_elements[sending_rounds][sb] = XOR( XOR(rx, ry),  XOR( XOR( AND(a.p1,a.p2) , AND(b.p1,b.p2) ) ,  AND(b.p1,b.p2) ) ); */ 
send_count[1] += 1;
a.p1 = maskP2;
a.p2 = maskP1_2;
}

OECL_Share complete_and(OECL_Share a, OECL_Share b)
{
return a;
}

void prepare_reveal_to_all(OECL_Share a)
{
    sending_args[0].sent_elements[sending_rounds][send_count[0]] = a.p1;
    sending_args[1].sent_elements[sending_rounds][send_count[1]] = a.p2;
    send_count[0] += 1;
    send_count[1] += 1;
}    



DATATYPE complete_Reveal(OECL_Share a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
DATATYPE result = XOR(a.p2,receiving_args[1].received_elements[rounds-1][share_buffer[1]]); 
share_buffer[1]+=1;
return result;
}

void send()
{
send_count[0] = 0;
send_count[1] = 0;
    for(int t = 0; t < num_players-1; t++)
        sending_args[t].sent_elements[sending_rounds + 1] = NEW(DATATYPE[sending_args[t].elements_to_send[sending_rounds + 1]]); // Allocate memory for all sending buffers for next round
    pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      /* printf("boradcasted round %i \n", sending_rounds); */
      pthread_mutex_unlock(&mtx_send_next); 
}

void receive(){
share_buffer[0] = 0;
share_buffer[1] = 0;
        rounds+=1;  
        // receive_data
      //wait until all sockets have finished received their last data
      pthread_mutex_lock(&mtx_receive_next);
      
std::chrono::high_resolution_clock::time_point c1 =
        std::chrono::high_resolution_clock::now();
      while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      
double time = std::chrono::duration_cast<std::chrono::microseconds>(
                     std::chrono::high_resolution_clock::now() - c1)
                     .count();
      /* printf("finished waiting for receive in round %i \n", rounds - 1); */
      pthread_mutex_unlock(&mtx_receive_next);
printf("Time spent waiting for data chrono: %fs \n", time / 1000000);

}

void communicate()
{
    send();
    receive();
}

OECL_Share* alloc_Share(int l)
{
    return new OECL_Share[l];
}



void prepare_receive_from(OECL_Share a[], int id, int l)
{
if(id == 0)
{
    if(optimized_sharing == true)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].p1 = player_input[share_buffer[2]];
    a[i].p2 = getRandomVal(0);
    sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(a[i].p2,player_input[share_buffer[2]]);
    send_count[1]+=1;
    share_buffer[2] += 1;
    /* DATATYPE r = getRandomVal(2); //should be an SRNG shared by P0,P1,P2 to save communication */
    /* a[i] = XOR(r,a[i]); */
    /* sending_args[0].sent_elements[sending_rounds][sb] = SET_ALL_ZERO(); */
    /* sending_args[1].sent_elements[sending_rounds][sb] = SET_ALL_ZERO(); */
    /* sb += 1; */
    /* a[i] = r; */
    }

    }
    else
    {
    for(int i = 0; i < l; i++)
    {
    a[i].p1 = getRandomVal(1);
    a[i].p2 = getRandomVal(0);
    sending_args[0].sent_elements[sending_rounds][send_count[0]] = XOR(a[i].p1,player_input[share_buffer[2]]);
    sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(a[i].p2,player_input[share_buffer[2]]);
    send_count[0]+=1;
    send_count[1]+=1;
    share_buffer[2] += 1;
    /* DATATYPE r = getRandomVal(2); //should be an SRNG shared by P0,P1,P2 to save communication */
    /* a[i] = XOR(r,a[i]); */
    /* sending_args[0].sent_elements[sending_rounds][sb] = SET_ALL_ZERO(); */
    /* sending_args[1].sent_elements[sending_rounds][sb] = SET_ALL_ZERO(); */
    /* sb += 1; */
    /* a[i] = r; */
    }

    }
}
else if(id == 1){
for(int i = 0; i < l; i++)
    {
    a[i].p1 = SET_ALL_ZERO();
    a[i].p2 = getRandomVal(0);
    }


}
else if(id == 2)// id ==2
{
    for(int i = 0; i < l; i++)
    {
    a[i].p1 = getRandomVal(1);
    a[i].p2 = SET_ALL_ZERO();
    }

}
}

void complete_receive_from(OECL_Share a[], int id, int l)
{
    return;
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
}
};
