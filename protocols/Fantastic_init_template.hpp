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
class Fantastic_Four_init
{
bool optimized_sharing;
public:
Fantastic_Four_init(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}


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

#if PARTY == 0

send_to_(P2);
send_to_(P1);
/* store_compare_view_init(P3); */

#elif PARTY == 1

send_to_(P3);
send_to_(P0);
/* store_compare_view_init(P0); */

#elif PARTY == 2

// c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
send_to_(P0);
/* store_compare_view_init(P1); */
/* store_compare_view_init(P1); */


#elif PARTY == 3

//c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
send_to_(P1);
/* store_compare_view_init(P2); */
/* store_compare_view_init(P0); */

#endif
}



void complete_and(DATATYPE &c)
{
#if PARTY == 0

//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + r013
receive_from_(P1);
store_compare_view_init(P3);
store_compare_view_init(P3);



//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
receive_from_(P2);
store_compare_view_init(P1);


#elif PARTY == 1

// c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
receive_from_(P3);
store_compare_view_init(P0);
store_compare_view_init(P2);

//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
receive_from_(P0);
store_compare_view_init(P2);

// receive second term from P0


#elif PARTY == 2


//c1 = a1 b1 + a1 b2 + (a1 b2 + a2 b1 - r013) + r023 + r023_2
receive_from_(P0);
store_compare_view_init(P1);
store_compare_view_init(P1);
store_compare_view_init(P3);
//receive rest from P0, verify with P3



#elif PARTY == 3


//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + (a0 b2 + a2 b0 - r123) + r013
receive_from_(P1);
store_compare_view_init(P0);
store_compare_view_init(P0);
store_compare_view_init(P2);
#endif
}



void prepare_reveal_to_all(DATATYPE a)
{
    send_to_(PNEXT);
}    



DATATYPE complete_Reveal(DATATYPE a)
{
receive_from_(PPREV);
store_compare_view_init(P0123);
return a;
}


DATATYPE* alloc_Share(int l)
{
    return new DATATYPE[l];
}



void prepare_receive_from(DATATYPE a[], int id, int l)
{
if(id == PSELF)
{
#if PARTY == 0
    for(int i = 0; i < l; i++)
    {
        send_to_(P1);
        send_to_(P2);
    }
#elif PARTY == 1
    for(int i = 0; i < l; i++)
    {
        send_to_(P0);
        send_to_(P2);
    }
#elif PARTY == 2
    for(int i = 0; i < l; i++)
    {
        send_to_(P0);
        send_to_(P1);
    }
#else // PARTY == 3
    for(int i = 0; i < l; i++)
    {
        send_to_(P0);
        send_to_(P1);
    }
#endif
}
}
void complete_receive_from(DATATYPE a[], int id, int l)
{
if(id != PSELF)
{
#if PARTY == 0
    if(id == P1)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P1);
    store_compare_view_init(P2);
    }
    }
    else if(id == P2)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P2);
    store_compare_view_init(P1);
    }
    }
    else // id == P3
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P3);
    store_compare_view_init(P1);
    }
    }
#elif PARTY == 1
    if(id == P0)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P0);
    store_compare_view_init(P2);
    }
    }
    else if(id == P2)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P2);
    store_compare_view_init(P0);
    }
    }
    else // id == P3
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P3);
    store_compare_view_init(P0);
    }
    }
#elif PARTY == 2
    if(id == P0)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P0);
    store_compare_view_init(P1);
    }
    }
    else if(id == P1)
    {
    for(int i = 0; i < l; i++)
    {
    receive_from_(P1);
    store_compare_view_init(P0);
    }
    } 
#endif
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

void finalize(std::string* ips)
{
    finalize_(ips);
}

void finalize(std::string* ips, receiver_args* ra, sender_args* sa)
{
    finalize_(ips, ra, sa);
}


};
