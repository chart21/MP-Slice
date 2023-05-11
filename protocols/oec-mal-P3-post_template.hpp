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
#include "init_protocol_base.hpp"
#include "oec-mal_base.hpp"
#include "live_protocol_base.hpp"
/* #define PRE_SHARE Dealer_Share */
class OEC_MAL3_POST
{
bool optimized_sharing;
public:
OEC_MAL3_POST(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

Dealer_Share public_val(DATATYPE a)
{
    return Dealer_Share(SET_ALL_ZERO(),SET_ALL_ZERO(),SET_ALL_ZERO());
}

Dealer_Share Not(Dealer_Share a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
Dealer_Share Xor(Dealer_Share a, Dealer_Share b)
{
   return a; 
}



//prepare AND -> send real value a&b to other P
void prepare_and(Dealer_Share a, Dealer_Share b, Dealer_Share &c)
{
}

void complete_and(Dealer_Share &c)
{
}


void prepare_reveal_to_all(Dealer_Share a)
{
}    



DATATYPE complete_Reveal(Dealer_Share a)
{
DATATYPE result = receive_from_live(P0);
store_compare_view(P0123, result);
return result;
}


Dealer_Share* alloc_Share(int l)
{
    return new Dealer_Share[l];
}



void prepare_receive_from(Dealer_Share a[], int id, int l)
{
}

void complete_receive_from(Dealer_Share a[], int id, int l)
{
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
