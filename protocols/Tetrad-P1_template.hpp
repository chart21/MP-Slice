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
#include "tetrad_base.hpp"
#include "live_protocol_base.hpp"
#define PRE_SHARE Tetrad_Share
class Tetrad1
{
bool optimized_sharing;
public:
Tetrad1(bool optimized_sharing) {this->optimized_sharing = optimized_sharing;}

Tetrad_Share public_val(DATATYPE a)
{
    return Tetrad_Share(a,SET_ALL_ZERO(),SET_ALL_ZERO());
}

Tetrad_Share Not(Tetrad_Share a)
{
   return Tetrad_Share(NOT(a.mv),a.l0,a.l1);
}

// Receive sharing of ~XOR(a,b) locally
Tetrad_Share Xor(Tetrad_Share a, Tetrad_Share b)
{
    a.mv = XOR(a.mv,b.mv);
    a.l0 = XOR(a.l0,b.l0);
    a.l1 = XOR(a.l1,b.l1);
   return a;
}



//prepare AND -> send real value a&b to other P
void prepare_and(Tetrad_Share a, Tetrad_Share b, Tetrad_Share &c)
{


DATATYPE y1ab = XOR( XOR(AND(a.l0,b.l1),AND(a.l1,b.l0)), AND(a.l1,a.l1));
DATATYPE u1 = getRandomVal(P013);

//q:
c.mv = SET_ALL_ZERO();
c.l0 = getRandomVal(P013); //lambda1
c.l1 = SET_ALL_ZERO();  //lambda3

DATATYPE s = getRandomVal(P123);

DATATYPE y2 = XOR( XOR(AND(a.l0,b.mv),AND(a.mv,b.l0)), XOR(y1ab,u1));
DATATYPE y3 = XOR(AND(a.l1,b.mv),AND(a.mv,b.l1));
send_to_live(P2, y2);
DATATYPE z_r = XOR( XOR(y2, y3), AND(a.mv,b.mv));

//Trick to store values neede later
c.storage = c.l0;
c.l0 = y2;
c.l1 = s;
c.mv = z_r;

}

void complete_and(Tetrad_Share &c)
{
    DATATYPE y1 = receive_from_live(P2);
    DATATYPE v = XOR(XOR(c.l0,c.l1),y1); // y1 + y2 + s for verification
    store_compare_view(P012, v);
    c.mv = XOR(c.mv, y1);

    //p:
    DATATYPE pl1 = SET_ALL_ZERO(); // known by all
    DATATYPE pl2 = SET_ALL_ZERO(); // known by all
    DATATYPE pl3 = getRandomVal(P123); //hide from P0
    DATATYPE pmv = XOR(c.mv,pl3);                                   //
    store_compare_view(P0, pmv);



    //o = p + q
    c.mv = pmv;
    c.l0 = c.storage; //lambda1
    c.l1 = pl3; //lambda3
}



void prepare_reveal_to_all(Tetrad_Share a)
{
}    



DATATYPE complete_Reveal(Tetrad_Share a)
{

//receive lambda3 from P3
store_compare_view(P0, a.l0); //help P0 verifying
store_compare_view(P3, a.mv); //help P3 verifying
                              
DATATYPE lambda2 = receive_from_live(P3);
store_compare_view(P0, lambda2); //get help from P0 to veriy
DATATYPE result = XOR(a.mv, lambda2);
result = XOR(result, a.l0);
result = XOR(result, a.l1);
return result;
}


Tetrad_Share* alloc_Share(int l)
{
    return new Tetrad_Share[l];
}



void prepare_receive_from(Tetrad_Share a[], int id, int l)
{
if(id == PSELF)
{
    for(int i = 0; i < l; i++)
    {
    a[i].mv = get_input_live();
    a[i].l0 = getRandomVal(P013); //l1
    a[i].l1 = getRandomVal(P123); //l3
    DATATYPE l2 = SET_ALL_ZERO();
    a[i].mv = XOR( XOR(a[i].mv, l2), XOR(a[i].l0,a[i].l1));
    send_to_live(P0, a[i].mv);
    send_to_live(P2, a[i].mv);

    
    
    } 
}
else if(id == P0)
{
    for(int i = 0; i < l; i++)
    {
    a[i].l0 = getRandomVal(P013);
    a[i].l1 = SET_ALL_ZERO();
    }
}
else if(id == P2)
{
    for(int i = 0; i < l; i++)
    {
    a[i].l0 = SET_ALL_ZERO();
    a[i].l1 = getRandomVal(P123);
    }
}
else if(id == P3)
{
    for(int i = 0; i < l; i++)
    {
    a[i].l0 = getRandomVal(P013);
    a[i].l1 = getRandomVal(P123);
    }
}
}
void complete_receive_from(Tetrad_Share a[], int id, int l)
{
if(id != PSELF)
{
    for(int i = 0; i < l; i++)
    {
    a[i].mv = receive_from_live(id);
    }

        if(id != P0)
            for(int i = 0; i < l; i++)
                store_compare_view(P0,a[i].mv);
        if(id != P2)
            for(int i = 0; i < l; i++)
                store_compare_view(P2,a[i].mv);


    
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
