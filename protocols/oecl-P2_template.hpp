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
#include "sharemind_base.hpp"
//
//#include "oecl_base.hpp"

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
    a = NOT(a);
   return a;
}

// Receive sharing of ~XOR(a,b) locally
XOR_Share Xor(XOR_Share a, XOR_Share b)
{
   return XOR(a,b);
}



//prepare AND -> send real value a&b to other P
void prepare_and(XOR_Share &a, XOR_Share &b)
{

XOR_Share ap1 = getRandomVal(0); // P2 mask for P1
a = XOR(receiving_args[0].received_elements[rounds-1][share_buffer[0]], AND(a,b)); // P0_message + (a+rr) (b+rl)
sending_args[1].sent_elements[sending_rounds][send_count[1]] = XOR(ap1,a); // + P2 mask
share_buffer[0]+=1;
send_count[1]+=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
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

void communicate()
{
    send_and_receive();
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

share_buffer[0] = 0;
share_buffer[1] = 0;
}

void send_and_receive()
{
    send();
    receive();
}

};
