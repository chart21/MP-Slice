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
class OECL0_init
{
bool optimized_sharing;
public:
OECL0_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}


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
void prepare_and(DATATYPE a, DATATYPE b, DATATYPE &c)
{
send_to_(P2);
}

// NAND both real Values to receive sharing of ~ (a&b) 
void complete_and(DATATYPE &c)
{
}

void prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < 2; t++) 
    {
        send_to_(t);
    }//add to send buffer
}    



DATATYPE complete_Reveal(DATATYPE a)
{
/* for(int t = 0; t < num_players-1; t++) */ 
/*     receiving_args[t].elements_to_rec[rounds-1]+=1; */
receive_from_(P2);
return a;
}


XOR_Share* alloc_Share(int l)
{
    return new DATATYPE[l];
}


void prepare_receive_from(DATATYPE a[], int id, int l)
{
/* return; */
/* old: */

if(id == P0)
{
if(optimized_sharing == true)
{
    for(int i = 0; i < l; i++)
    {
        send_to_(P2);
    }

}
else{
    for(int i = 0; i < l; i++)
    {
        send_to_(P1);
        send_to_(P2);
    }

}

}
}

void complete_receive_from(DATATYPE a[], int id, int l)
{
    return;
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
