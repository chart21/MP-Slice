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
//#define XOR_Share DATATYPE

class TTP
{
bool input_srngs;
    public:
TTP(bool use_srngs) {input_srngs = use_srngs;}

DATATYPE share(DATATYPE a)
{
sending_args[1].sent_elements[sending_rounds][sb] = a;
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
    result = receiving_args[1].received_elements[rounds-1][rb];
    rb+=1;
}

return result;
}


void send()
{
sb = 0;      
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

rb = 0;
}

void send_and_receive()
{
    send();
    receive();
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == player_id && player_id != 2)
{
    for(int s = 0; s < l; s++)
    {
        a[s] = share(player_input[share_buffer[id]]);  
        share_buffer[id]+=1;
    }
}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{

if(id != player_id && player_id == 2)
{
for (int i = 0; i < l; i++) {
    a[i] = receiving_args[id].received_elements[rounds-1][share_buffer[id]];
    share_buffer[id]+=1;
    }
}
}



XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}

void communicate()
{
    send();
    receive();
}   


void finalize()
{

}


};
