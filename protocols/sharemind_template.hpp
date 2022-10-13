#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
//#define pnext player_id == 0 ? 0 : player_id == 1 ? 1 : 0 
/* #define pnext (player_id + 1)%3 */
//#define pprev player_id == 0 ? 1 : player_id == 1 ? 0 : 1 
/* #define pprev (player_id - 1)%3 */
// Share of each player is ~a
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"
#define Share DATATYPE

class Sharemind
{
    public:
Share share_SRNG(DATATYPE a)
{
DATATYPE s[3]; //last share always belongs to player itself
s[pprev] = getRandomVal(pprev);
s[pnext] = getRandomVal(pnext);
s[2] = XOR(s[pprev],s[pnext]);
s[2] = XOR(a,s[2]);
return s[2];
}

Share receive_share_SRNG(int player)
{
    return getRandomVal(player);
}

DATATYPE share(DATATYPE a)
{
DATATYPE s[3]; //last share always belongs to player itself
/* s[pprev] = getRandomVal(pprev); */ 
/* s[pnext] = getRandomVal(pnext); */
/* s[pprev] = SET_ALL_ONE(); */
/* s[pnext] = SET_ALL_ONE(); */
s[pprev] = getRandomVal(num_players -1);
s[pnext] = getRandomVal(num_players -1);
s[2] = XOR(s[pprev],s[pnext]);
s[2] = XOR(a,s[2]);
sending_args[pprev].sent_elements[sending_rounds][sb] = s[pprev];
sending_args[pnext].sent_elements[sending_rounds][sb] = s[pnext];
sb+=1;
/* return a; */
/* print_num(s[player_id]); */
/* printf("\n"); */
/* print_num(a); */
/* printf("\n"); */
/* printf("\n"); */
return s[2];
      }





void share(DATATYPE a[], int length)
{
    for(int l = 0; l < length; l++)
        a[l] = share(a[l]);
                                           //
}
Share public_val(DATATYPE a)
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



void reshare(DATATYPE a, DATATYPE u[])
{
u[pprev] = getRandomVal(pprev);
u[pnext] = getRandomVal(pnext);
u[2] = XOR(u[pprev],u[pnext]);
u[2] = XOR(a,u[2]);
 
}
//prepare AND -> send real value a&b to other P
void prepare_and(Share &a, Share &b)
{
DATATYPE u[3];
DATATYPE v[3];
reshare(a,u);
reshare(b,v);
a = u[2];
b = v[2];
sending_args[pnext].sent_elements[sending_rounds][sb] = u[2]; //last share always belongs to player itself
sending_args[pprev].sent_elements[sending_rounds][sb] = v[2];
sb+=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share complete_and(Share a, Share b)
{
DATATYPE w = AND(a,b);
DATATYPE u_p = receiving_args[pprev].received_elements[rounds-1][rb];
DATATYPE v_n = receiving_args[pnext].received_elements[rounds-1][rb];
/* DATATYPE u_i = a; */
DATATYPE v_i = b; 
w = XOR (w,   XOR ( AND(u_p,v_i) , AND(u_p,v_n) ));
rb+=1;
return w;
}

void prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
        sending_args[t].sent_elements[sending_rounds][sb] = a;
    sb += 1;
    //add to send buffer
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
DATATYPE result = a;
for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
    result = XOR(result,receiving_args[t].received_elements[rounds-1][rb]);
rb+=1;
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
      printf("boradcasted round %i \n", sending_rounds);
      pthread_mutex_unlock(&mtx_send_next); 
}

void receive(){
        rounds+=1;  
        // receive_data
      //wait until all sockets have finished received their last data
      pthread_mutex_lock(&mtx_receive_next);
      while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      printf("finished waiting for receive in round %i \n", rounds - 1);
      pthread_mutex_unlock(&mtx_receive_next);

rb = 0;
}

void send_and_receive()
{
    send();
    receive();
}
void receive_from_SRNG(Share a[], int id, int l)
{
if(id == player_id)
{
for (int i = 0; i < l; i++) {
  a[i] = share_SRNG(player_input[share_buffer[id]]);  
  share_buffer[id]+=1;
}
}
else{
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    a[i] = receive_share_SRNG(id - offset);
}
}
}
void receive_from(DATATYPE a[], int id, int l)
{
if(id == player_id)
{
/* result = receiving_args[num_players-1].received_elements[rounds - 1][share_buffer[id]]; */
for (int i = 0; i < l; i++) {
  a[i] = player_input[share_buffer[id]];
    share_buffer[id]+=1;
}
}
else{
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
a[i] = receiving_args[id - offset].received_elements[rounds-1][share_buffer[id]];
    share_buffer[id]+=1;
}
}
}

Share* alloc_Share(int l)
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
