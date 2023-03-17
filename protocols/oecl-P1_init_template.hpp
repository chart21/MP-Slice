#pragma once
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
#include "init_protocol_base.hpp"
class OECL1_init
{
bool optimized_sharing;
public:
OECL1_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}


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



//prepare AND -> send real value a&b to other P
void prepare_and(DATATYPE &a, DATATYPE &b)
{
sending_args[1].elements_to_send[sending_args[1].send_rounds] += 1;
//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE complete_and(DATATYPE a, DATATYPE b)
{
receiving_args[1].elements_to_rec[receiving_args[1].rec_rounds -1] += 1;
return a;
}

void prepare_reveal_to_all(DATATYPE a)
{
return;
}    



DATATYPE complete_Reveal(DATATYPE a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
receiving_args[0].elements_to_rec[receiving_args[0].rec_rounds -1]+=1;
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == 1)
{
    for(int i = 0; i < l; i++)
    {
        sending_args[1].elements_to_send[sending_args[1].send_rounds] += 1;
    }

}
}


void complete_receive_from(DATATYPE a[], int id, int l)
{
if(id == 2)
{
for(int i = 0; i < l; i++)
    receiving_args[1].elements_to_rec[receiving_args[1].rec_rounds -1] += 1;
}
else if(id == 0)
{
if(optimized_sharing == true)
{
    return;
}
else
{
for(int i = 0; i < l; i++)
    receiving_args[0].elements_to_rec[receiving_args[0].rec_rounds -1] += 1;
}
} 
/* if(id == player_id) */
/*     return; */
/* int offset = {id > player_id ? 1 : 0}; */
/* int player = id - offset; */
/* for(int i = 0; i < l; i++) */
/*     receiving_args[player].elements_to_rec[receiving_args[player].rec_rounds -1] += 1; */
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

void finalize(std::string* ips)
{
    finalize_(ips);
}


};
