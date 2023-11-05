#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"

#include "../utils/randomizer.h"


struct Share{
    DATATYPE x;
    DATATYPE a;
};

Share P_share_SRNG(DATATYPE a)
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

Share P_receive_share_SRNG(int player)
{
Share s;
s.x = getRandomVal(player);
s.a = receiving_args[player].received_elements[rounds-1][share_buffer[player]];
share_buffer[player] += 1;
return s;
}

DATATYPE P_share(DATATYPE a)
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





void P_share(DATATYPE a[], int length)
{
    for(int l = 0; l < length; l++)
        a[l] = P_share(a[l]);
                                           //
}



// Receive sharing of ~XOR(a,b) locally
Share P_xor(Share a, Share b)
{
    Share result;
    result.x = XOR(a.x,b.x);
    result.a = XOR(a.a,b.a);
    return result; 
}

Share P_xor_pub(Share a, DATATYPE b)
{
    Share result;
    result.x = a.x;
    result.a = XOR(a.a,b);
    return result; 
}

Share P_public_val(DATATYPE a)
{
    Share result;
    result.x = 0;
    result.a = a;
    return result; 
}

Share P_not(Share a)
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
void P_prepare_and(Share* a, Share* b)
{
DATATYPE corr = XOR( getRandomVal(pprev), getRandomVal(pnext) );
DATATYPE r =  XOR( XOR(  AND(a->x,b->x), AND(a->a,b->a) ) , corr);  
a->x = r; //used to access value in complete and 
sending_args[pnext].sent_elements[sending_rounds][sb] = r; //last share always belongs to player itself
sb+=1;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share P_complete_and(Share a, Share b)
{
Share result;
result.x = a.a; 
result.a = XOR(result.x, receiving_args[pprev].received_elements[rounds-1][rb]  );
rb+=1;
return result;
}

void P_prepare_reveal_to_all(Share a)
{
    sending_args[pprev].sent_elements[sending_rounds][sb] = a.x;
    sb += 1;
    //add to send buffer
}    


//reveal to specific player
void P_prepare_reveal_to(DATATYPE a, int id)
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

DATATYPE P_complete_Reveal(Share a)
{
DATATYPE result;
result = XOR(a.a,receiving_args[pnext].received_elements[rounds-1][rb]);
rb+=1;
return result;
}

void P_communicate_AND()
{
    // send all AND gates
}

void P_communicate_Shares()
{
    // send all reveals
}

