#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"
#include "replicated_base.hpp"


class Replicated{
bool input_srngs;
    public:
Replicated(bool use_srngs) {input_srngs = use_srngs;}
Share share_SRNG(DATATYPE a)
{
Share s[3];
s[pprev].x = getRandomVal(pprev);
s[pnext].x = getRandomVal(pnext);
s[2].x = XOR(s[pprev].x,s[pnext].x);

s[pprev].a = XOR(s[pnext].x,a); //xi + x(i-1) + a
s[pnext].a = XOR(s[2].x,a); //xi + x(i-1) + a
s[2].a = XOR(s[pprev].x,a); //xi + x(i-1) + a

sending_args[pprev].sent_elements[sending_rounds][sb] = s[pprev].a;
sending_args[pnext].sent_elements[sending_rounds][sb] = s[pnext].a;
sb+=1;

return s[2];


}

void receive_share_SRNG(Share &s, int player)
{
s.a = receiving_args[player].received_elements[rounds-1][share_buffer[player]];
share_buffer[player] += 1;
}

Share share(DATATYPE a)
{
Share s[3];
s[pprev].x = getRandomVal(pprev);
s[pnext].x = getRandomVal(pnext);
s[2].x = XOR(s[pprev].x,s[pnext].x);

s[pprev].a = XOR(s[pnext].x,a); //xi + x(i-1) + a
s[pnext].a = XOR(s[2].x,a); //xi + x(i-1) + a
s[2].a = XOR(s[pprev].x,a); //xi + x(i-1) + a

sending_args[pprev].sent_elements[sending_rounds][sb] = s[pprev].a;
sending_args[pnext].sent_elements[sending_rounds][sb] = s[pnext].a;
sb+=1;

return s[2];
      }





void share(Share a[], int length)
{
    for(int l = 0; l < length; l++)
    {
    a[l] = share_SRNG(player_input[share_buffer[2]]);  
    share_buffer[2]+=1;
    }                                      //
}


void receive_from_SRNG(Share a[], int id, int l)
{
if(id == player_id)
{
for (int i = 0; i < l; i++) {
  a[i] = share_SRNG(player_input[share_buffer[2]]);  
  share_buffer[2]+=1;
}
}
else{
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    receive_share_SRNG(a[i], id - offset);
}
}
}

void receive_from(Share a[], int id, int l)
{
if(id == player_id)
{
/* for (int i = 0; i < l; i++) { */

/* a[i] = share(player_input[share_buffer[id]]); */
/* share_buffer[player_id] += 1; */

/* } */
    return;
}
else{
int offset = {id > player_id ? 1 : 0};
int player = id - offset;
for (int i = 0; i < l; i++) {
Share s;
s.x = getRandomVal(player);
s.a = receiving_args[player].received_elements[rounds-1][share_buffer[player]];
share_buffer[player] += 1;
a[i] = s;
}
}
}

void generate_SRNG(Share a[], int id, int length)
{
int offset = {id > player_id ? 1 : 0};
int player = id - offset;
    for(int l = 0; l < length; l++)
    {
        a[l].x = getRandomVal(player);
    }
}

void prepare_receive_from(Share a[], int id, int l)
{
    if(id == player_id)
        share(a,l);
    else
        generate_SRNG(a,id,l);
}

void complete_receive_from(Share a[], int id, int l)
{
if(id == player_id)
    return;
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    receive_share_SRNG(a[i],id - offset);

}
}

// Receive sharing of ~XOR(a,b) locally
Share Xor(Share a, Share b)
{
    Share result;
    result.x = XOR(a.x,b.x);
    result.a = XOR(a.a,b.a);
    return result; 
}

Share Xor_pub(Share a, DATATYPE b)
{
    Share result;
    result.x = a.x;
    result.a = XOR(a.a,b);
    return result; 
}

Share public_val(DATATYPE a)
{
    Share result;
    result.x = SET_ALL_ZERO();
    result.a = a;
    return result; 
}

Share Not(Share a)
{
    Share result;
    result.x = a.x;
    result.a = NOT(a.a);
    return result; 
}


void reshare(DATATYPE a, DATATYPE u[])
{
//u[pprev] = getRandomVal(pprev); //replace with SRNG
//u[pnext] = getRandomVal(pnext);
u[pprev] = getRandomVal(pprev);
u[pnext] = getRandomVal(pnext);

/* u[pprev] = SET_ALL_ONE(); */
/* u[pnext] = SET_ALL_ONE(); */
/* if(player_id == 0) */
/*     u[pnext] = SET_ALL_ZERO(); */
/* if(player_id == 1) */
/*     u[pprev] = SET_ALL_ZERO(); */

    /* u[pprev] = SET_ALL_ZERO(); //replace with SRNG */
/* u[pnext] = SET_ALL_ONE(); */
u[2] = XOR(u[pprev],u[pnext]);
u[2] = XOR(a,u[2]);
 
}
//prepare AND -> send real value a&b to other P
void prepare_and(Share &a, Share &b)
{
DATATYPE corr = XOR( getRandomVal(pprev), getRandomVal(pnext) );
DATATYPE r =  XOR( XOR(  AND(a.x,b.x), AND(a.a,b.a) ) , corr);  
a.x = r; //used to access value in complete and 
sending_args[pnext].sent_elements[sending_rounds][sb] = r; //last share always belongs to player itself
sb+=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share complete_and(Share a, Share b)
{
Share result;
result.a = a.x; // c = ri 
result.x = XOR(a.x, receiving_args[pprev].received_elements[rounds-1][rb]  ); // z = ri XOR rprev 
rb+=1;
return result;
}

void prepare_reveal_to_all(Share a)
{
    sending_args[pnext].sent_elements[sending_rounds][sb] = a.x;
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

DATATYPE complete_Reveal(Share a)
{
DATATYPE result;
result = XOR(a.a,receiving_args[pprev].received_elements[rounds-1][rb]);
rb+=1;
return result;
}

void communicate_AND()
{
    // send all AND gates
}

void communicate_Shares()
{
    // send all reveals
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
      /* printf("finished waiting for receive in round %i \n", rounds - 1); */
      pthread_mutex_unlock(&mtx_receive_next);

rb = 0;
}

void send_and_receive()
{
    send();
    receive();
}
Share* alloc_Share(int l)
{
    return new Share[l];
}

void communicate()
{
    send();
    receive();
}   
};
