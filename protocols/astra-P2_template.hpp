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
//#include "sharemind_base.hpp"
#include "astra_base.hpp"
#include "live_protocol_base.hpp"
class ASTRA2
{
bool optimized_sharing;
public:
ASTRA2(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

Evaluator_Share public_val(DATATYPE a)
{
    return Evaluator_Share(a,SET_ALL_ZERO());
}

Evaluator_Share Not(Evaluator_Share a)
{
    a.mv = NOT(a.mv);
   return a;
}

// Receive sharing of ~XOR(a,b) locally
Evaluator_Share Xor(Evaluator_Share a, Evaluator_Share b)
{
   return Evaluator_Share(XOR(a.mv,b.mv),XOR(a.lv,b.lv));
}



//prepare AND -> send real value a&b to other P
void prepare_and(Evaluator_Share &a, Evaluator_Share &b)
{
DATATYPE yz2 = getRandomVal(0); //yz1
DATATYPE yxy2 = receiving_args[0].received_elements[rounds-1][share_buffer[0]];
share_buffer[0] +=1;
a.mv = XOR( AND(a.mv,b.mv), XOR( XOR(  XOR( AND(a.mv,b.lv), AND(b.mv, a.lv) ), yz2 ), yxy2)); 
sending_args[1].sent_elements[sending_rounds][send_count[1]] = a.mv; 
send_count[1]+=1;
a.lv = yz2;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Evaluator_Share complete_and(Evaluator_Share a, Evaluator_Share b)
{
// a.p2 already set in last round
a.mv = XOR(a.mv, receiving_args[1].received_elements[rounds-1][share_buffer[1]]);
share_buffer[1]+=1;
return a; 
}

void prepare_reveal_to_all(Evaluator_Share a)
{
sending_args[0].sent_elements[sending_rounds][send_count[0]] = a.mv; 
send_count[0]+=1;
}    



DATATYPE complete_Reveal(Evaluator_Share a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */

a.mv = XOR(a.mv,receiving_args[0].received_elements[rounds-1][share_buffer[0]]); 
share_buffer[0]+=1;
return a.mv;
}


Evaluator_Share* alloc_Share(int l)
{
    return new Evaluator_Share[l];
}


void prepare_receive_from(Evaluator_Share a[], int id, int l)
{
if(id == 0)
{
    if(optimized_sharing == false)
    {
    for(int i = 0; i < l; i++)
    {
        a[i].lv = getRandomVal(0);
    }


    }
}
else if(id == 2) // -> lv = lv1, lv2=0
{
for(int i = 0; i < l; i++)
{
    a[i].lv = getRandomVal(0);
    a[i].mv = XOR(player_input[share_buffer[2]],a[i].lv);
    share_buffer[2] += 1;
    sending_args[1].sent_elements[sending_rounds][send_count[1]] = a[i].mv;
    send_count[1]+=1;
}
}
}

void complete_receive_from(Evaluator_Share a[], int id, int l)
{
if(id == 0)
{
if(optimized_sharing == true) // (0,a+yx1)
{
    for(int i = 0; i < l; i++)
    {
        a[i].mv = SET_ALL_ZERO(); //Check options 
        a[i].lv = receiving_args[0].received_elements[rounds-1][share_buffer[0]];
        share_buffer[0] +=1;
    }
}
else{
    for(int i = 0; i < l; i++)
    {
        a[i].mv = receiving_args[0].received_elements[rounds-1][share_buffer[0]];
        share_buffer[0] +=1;
    }
    
}
}
else if(id == 1)
{
for(int i = 0; i < l; i++)
{
a[i].mv = receiving_args[1].received_elements[rounds-1][share_buffer[1]];
share_buffer[1] +=1;
/* a[i].p1 = SET_ALL_ZERO(); */
a[i].lv = SET_ALL_ZERO();
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
