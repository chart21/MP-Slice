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
#include "fantastic_base.hpp"
#include "live_protocol_base.hpp"
#define PRE_SHARE Fantastic_Share
class Fantastic_Four
{
bool optimized_sharing;
public:
Fantastic_Four(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

Fantastic_Share public_val(DATATYPE a)
{
    #if PARTY == 0
    return Fantastic_Share(SET_ALL_ZERO(),SET_ALL_ZERO(),SET_ALL_ZERO());
    #else
    return Fantastic_Share(a,SET_ALL_ZERO() ,SET_ALL_ZERO()); // a + a + a + 0 + 0 ... = a (Valid for XOR)
    #endif
}

Fantastic_Share Not(Fantastic_Share a)
{
#if PARTY < 3
    return Fantastic_Share(a.v0,a.v1,NOT(a.v2));
#else
    return a;
#endif
   /* return Fantastic_Share(NOT(a.v0),NOT(a.v1),NOT(a.v2)); */
}

// Receive sharing of ~XOR(a,b) locally
Fantastic_Share Xor(Fantastic_Share a, Fantastic_Share b)
{
    return Fantastic_Share(XOR(a.v0,b.v0),XOR(a.v1,b.v1),XOR(a.v2,b.v2));
}



//prepare MULT -> send real value a&b to other P
void prepare_and(Fantastic_Share a, Fantastic_Share b, Fantastic_Share &c)
{
DATATYPE send_valu = SET_ALL_ZERO();

/* c.v1 = XOR (XOR(cross_term1,cross_term3), AND(a.v1, b.v1)); */
/* c.v2 = XOR (XOR(cross_term2,cross_term3), AND(a.v2, b.v2)); */

#if PARTY == 0

#elif PARTY == 1



#elif PARTY == 2

send_to_live(P0, send_valu);


#elif PARTY == 3


//c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
send_to_live(P1, send_valu);

#endif
}



void complete_and(Fantastic_Share &c)
{

DATATYPE receive_term0 = SET_ALL_ZERO();
DATATYPE receive_term1 = SET_ALL_ZERO();
#if PARTY == 0

store_compare_view(P3,c.verify_store0);
//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + r013
store_compare_view(P3,receive_term1);
c.v1 = XOR(c.v1, receive_term1);


//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
DATATYPE receive_term2 = receive_from_live(P2);
store_compare_view(P1,receive_term2);
c.v2 = XOR(c.v2, receive_term2);


#elif PARTY == 1

// c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
store_compare_view(P0,c.verify_store0);

DATATYPE receive_term3 = receive_from_live(P3);
store_compare_view(P2,receive_term3);
c.v0 = XOR(c.v0, receive_term3);

//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
store_compare_view(P2,receive_term0);
c.v2 = XOR(c.v2, receive_term0);

// receive second term from P0


#elif PARTY == 2

store_compare_view(P1,c.verify_store0);
store_compare_view(P1,c.verify_store1);
//c1 = a1 b1 + a1 b2 + (a1 b2 + a2 b1 - r013) + r023 + r023_2
store_compare_view(P3,receive_term0);
c.v1 = XOR(c.v1, receive_term0);
//receive rest from P0, verify with P3



#elif PARTY == 3


//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + (a0 b2 + a2 b0 - r123) + r013
store_compare_view(P0,receive_term1);
c.v2 = XOR(c.v2, receive_term1);
store_compare_view(P0,c.verify_store1);
store_compare_view(P2,c.verify_store0);
#endif
}

#if FUNCTION_IDENTIFIER > 4
void prepare_mult(Fantastic_Share a, Fantastic_Share b, Fantastic_Share &c)
{
#if PARTY == 0
DATATYPE send_valu = SET_ALL_ZERO();
/* send_to_live(P2, send_valu); */
/* send_to_live(P1, send_valu); */
#elif PARTY == 1

send_to_live(P3, send_valu);
send_to_live(P0, send_valu);

#elif PARTY == 2

send_to_live(P0, send_valu);


#elif PARTY == 3

send_to_live(P1, send_valu);

#endif
}



void complete_mult(Fantastic_Share &c)
{
#if PARTY == 0

store_compare_view(P3,c.verify_store0);
//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + r013
DATATYPE receive_term1 = receive_from_live(P1);
store_compare_view(P3,receive_term1);
c.v1 = ADD(c.v1, receive_term1);


//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
DATATYPE receive_term2 = receive_from_live(P2);
store_compare_view(P1,receive_term2);
c.v2 = ADD(c.v2, receive_term2);


#elif PARTY == 1

// c0 = a0 b0 + (a0 b1 + a1 b0 - r023) + r123 + r123_2
store_compare_view(P0,c.verify_store0);

DATATYPE receive_term3 = receive_from_live(P3);
store_compare_view(P2,receive_term3);
c.v0 = ADD(c.v0, receive_term3);

//c3 = a3 b3 + a3 b2 + (a3 b0 + a0 b3 - r123) + (a1b3 + a3b1 - r023) + r012
DATATYPE receive_term0 = receive_from_live(P0);
store_compare_view(P2,receive_term0);
c.v2 = ADD(c.v2, receive_term0);

// receive second term from P0


#elif PARTY == 2

store_compare_view(P1,c.verify_store0);
store_compare_view(P1,c.verify_store1);
//c1 = a1 b1 + a1 b2 + (a1 b2 + a2 b1 - r013) + r023 + r023_2
DATATYPE receive_term0 = receive_from_live(P0);
store_compare_view(P3,receive_term0);
c.v1 = ADD(c.v1, receive_term0);
//receive rest from P0, verify with P3



#elif PARTY == 3


//c2 = a2 b2 + a2 b1 + (a2 b3 + a3 b2 - r012) + (a0 b2 + a2 b0 - r123) + r013
DATATYPE receive_term1 = receive_from_live(P1);
store_compare_view(P0,receive_term1);
c.v2 = ADD(c.v2, receive_term1);
store_compare_view(P0,c.verify_store1);
store_compare_view(P2,c.verify_store0);
#endif
}

#endif


void prepare_reveal_to_all(Fantastic_Share a)
{
#if PARTY == 0
send_to_live(P1, a.v0);
#elif PARTY == 1
send_to_live(P2, a.v1);
#elif PARTY == 2
send_to_live(P3, a.v2);
#else
send_to_live(P0, a.v0);
#endif
}    



DATATYPE complete_Reveal(Fantastic_Share a)
{
#if FUNCTION_IDENTIFIER > 4
DATATYPE result = ADD( ADD(a.v1,a.v2) ,ADD(a.v0, receive_from_live(PPREV)));
#else
DATATYPE result = XOR( XOR(a.v1,a.v2) ,XOR(a.v0, receive_from_live(PPREV)));
#endif
store_compare_view(P0123, result);
return result;
}


Fantastic_Share* alloc_Share(int l)
{
    return new Fantastic_Share[l];
}



void prepare_receive_from(Fantastic_Share a[], int id, int l)
{
if(id == PSELF)
{
#if PARTY == 0
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P023);
    a[i].v1 = getRandomVal(P013);
    a[i].v2 = XOR(get_input_live() , XOR(a[i].v0, a[i].v1));
    send_to_live(P1, a[i].v2);
    send_to_live(P2, a[i].v2);
    }
#elif PARTY == 1
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = getRandomVal(P013);
    a[i].v2 = XOR(get_input_live() , XOR(a[i].v0, a[i].v1));
    send_to_live(P0, a[i].v2);
    send_to_live(P2, a[i].v2);
    }
#elif PARTY == 2
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = getRandomVal(P023);
    a[i].v2 = XOR(get_input_live() , XOR(a[i].v0, a[i].v1));
    send_to_live(P0, a[i].v2);
    send_to_live(P1, a[i].v2);
    }
#else // PARTY == 3
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = getRandomVal(P023);
    a[i].v2 = XOR(get_input_live() , XOR(a[i].v0, a[i].v1));
    send_to_live(P0, a[i].v2);
    send_to_live(P1, a[i].v2);
    }
#endif
}
else{
if(id == P0)
{
 #if PARTY == 1
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = SET_ALL_ZERO();
    a[i].v1 = getRandomVal(P013);
    // receive
    }
#elif PARTY == 2
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = SET_ALL_ZERO();
    a[i].v1 = getRandomVal(P023);
    // receive
    }
#else // PARTY == 3
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = SET_ALL_ZERO();
    a[i].v1 = getRandomVal(P023);
    a[i].v2 = getRandomVal(P013);
    }
#endif
}
else if(id == P1)
{
 #if PARTY == 0
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = SET_ALL_ZERO();
    a[i].v1 = getRandomVal(P013);
    // receive
    }
#elif PARTY == 2
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = SET_ALL_ZERO();
    // receive
    }
#else // PARTY == 3
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = SET_ALL_ZERO();
    a[i].v2 = getRandomVal(P013);
    }
#endif
}
else if(id == P2)
{
 #if PARTY == 0
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P023);
    a[i].v1 = SET_ALL_ZERO();
    // receive
    }
#elif PARTY == 1
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = SET_ALL_ZERO();
    // receive
    }
#else // PARTY == 3
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = getRandomVal(P023);
    a[i].v2 = SET_ALL_ZERO();
    }
#endif
}
else // id == P3
{
 #if PARTY == 0
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P023);
    a[i].v2 = SET_ALL_ZERO();
    // receive
    }
#elif PARTY == 1
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    //receive
    a[i].v2 = SET_ALL_ZERO();
    }
#else // PARTY == 2
    for(int i = 0; i < l; i++)
    {
    a[i].v0 = getRandomVal(P123);
    a[i].v1 = getRandomVal(P023);
    a[i].v2 = SET_ALL_ZERO();
    }
#endif
}
}
}
void complete_receive_from(Fantastic_Share a[], int id, int l)
{
if(id != PSELF)
{
#if PARTY == 0
    if(id == P1)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P1);
    store_compare_view(P2,a[i].v2);
    }
    }
    else if(id == P2)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P2);
    store_compare_view(P1,a[i].v2);
    }
    }
    else // id == P3
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v1 = receive_from_live(P3);
    store_compare_view(P1,a[i].v1);
    }
    }
#elif PARTY == 1
    if(id == P0)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P0);
    store_compare_view(P2,a[i].v2);
    }
    }
    else if(id == P2)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P2);
    store_compare_view(P0,a[i].v2);
    }
    }
    else // id == P3
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v1 = receive_from_live(P3);
    store_compare_view(P0,a[i].v1);
    }
    }
#elif PARTY == 2
    if(id == P0)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P0);
    store_compare_view(P1,a[i].v2);
    }
    }
    else if(id == P1)
    {
    for(int i = 0; i < l; i++)
    {
    a[i].v2 = receive_from_live(P1);
    store_compare_view(P0,a[i].v2);
    }
    } 
#endif
}
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
/* #if PRE == 0 */
    communicate_live();
/* #endif */
}

};
