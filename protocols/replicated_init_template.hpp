#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"

#include "../utils/randomizer.h"
#include "sharemind_base.hpp"
#include "init_protocol_base.hpp"
class Replicated_init{
bool input_srngs;
    public:
Replicated_init(bool use_srngs) {input_srngs = use_srngs;}



XOR_Share share_SRNG(DATATYPE a)
{

sending_args[pprev].elements_to_send[sending_args[pprev].send_rounds] +=1;
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
XOR_Share dummy;
return dummy;
}

XOR_Share receive_share_SRNG(int player)
{
receiving_args[player].elements_to_rec[receiving_args[player].rec_rounds - 1] +=1;
XOR_Share s;
return s;
}



void share(XOR_Share a[], int length)
{

    for(int l = 0; l < length; l++)
    {
    share_SRNG(player_input[share_buffer[player_id]]);  
}
}


void prepare_receive_from(XOR_Share a[], int id, int l)
{
    if(id == player_id)
        share(a,l);
}

void complete_receive_from(XOR_Share a[], int id, int l)
{
if(id == player_id)
    return;
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
    a[i] = receive_share_SRNG(id - offset);

}
}


// Receive sharing of ~XOR(a,b) locally
XOR_Share Xor(XOR_Share a, XOR_Share b)
{
    return a; 
}

XOR_Share Xor_pub(XOR_Share a, DATATYPE b)
{
    return a; 
}

XOR_Share public_val(DATATYPE a)
{
    XOR_Share dummy;
    return dummy; 
}

XOR_Share Not(XOR_Share a)
{
    return a; 
}


//prepare AND -> send real value a&b to other P
void prepare_and(XOR_Share a, XOR_Share b, XOR_Share &c)
{
sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(XOR_Share &c)
{
receiving_args[pprev].elements_to_rec[receiving_args[pprev].rec_rounds - 1] +=1;
}

void prepare_reveal_to_all(XOR_Share a)
{
    sending_args[pnext].elements_to_send[sending_args[pnext].send_rounds] +=1;
    //add to send buffer
}    



DATATYPE complete_Reveal(XOR_Share a)
{
DATATYPE result;
receiving_args[pprev].elements_to_rec[receiving_args[pprev].rec_rounds - 1] +=1;
return result;
}


XOR_Share* alloc_Share(int l)
{
    return new XOR_Share[l];
}

void receive_from_SRNG(XOR_Share a[], int id, int l)
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
void receive_from(XOR_Share a[], int id, int l)
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

void finalize_ips(std::string* ips)
{
    finalize_ips_(ips);
}

};
