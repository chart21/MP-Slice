#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
//#define pnext player_id == 0 ? 0 : player_id == 1 ? 1 : 0 
/* #define pnext (player_id + 1)%3 */
//#define pprev player_id == 0 ? 1 : player_id == 1 ? 0 : 1 
/* #define pprev (player_id - 1)%3 */
// Share of each player is ~a

#include "../utils/randomizer.h"

DATATYPE P_share_SRNG(DATATYPE a)
{
DATATYPE s[num_players-1]; //last share always belongs to player itself
s[num_players-2] = a;
for(int t = 0; t < num_players-2; t++)
{
    s[t] = getRandomVal(t);
    s[num_players-2] = XOR(s[num_players-2], s[t]);
}
return s[num_players -2];
}

// Receive sharing of ~XOR(a,b) locally
inline DATATYPE P_xor(DATATYPE a, DATATYPE b)
{
   return XOR(a, b);
}

//prepare AND -> send real value a&b to other P
void P_prepare_and(DATATYPE* a, DATATYPE* b)
{
DATATYPE d = XOR(*a, getRandomVal(num_players -1));
DATATYPE e = XOR(*b, getRandomVal(num_players -1));
// anounce shares
for(int t = 0; t < num_players-2; t++)
{
    sending_args[t].sent_elements[sending_rounds][sb] = d;
    sending_args[t].sent_elements[sending_rounds][sb+1] = e;
}
sb+=2;
receiving_args[num_players-1].received_elements[rounds-1][rb] = d;
receiving_args[num_players-1].received_elements[rounds-1][rb + e] = e; //Hack, save d,e in receive buffer of trusted dealer
rb+=2;
//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE P_complete_and(DATATYPE a, DATATYPE b)
{
//recover saved shares of d and e from receiving buffer
DATATYPE d = receiving_args[num_players-1].received_elements[rounds-1][rb];
DATATYPE e = receiving_args[num_players-1].received_elements[rounds-1][rb];
rb+=2;
//open d,e
for(int t = 0; t < num_players-2; t++)
{
d = XOR(receiving_args[num_players-1].received_elements[rounds-1][rb],d);
e = XOR(receiving_args[num_players-1].received_elements[rounds-1][rb],e);
}
rb+=2;
DATATYPE c;
if(player_id == 0)
{
    c = receiving_args[num_players-1].received_elements[rounds-1][rb]; //get share from ttp
rb+=1;
}
else
    c = getRandomVal(num_players -1);
return XOR( XOR( AND(d,e), AND(d,b) ) , XOR (AND(e,a), c)) ;
}

void P_prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < num_players-2; t++) // for other protocols, sending buffers may be different for each player
        sending_args[t].sent_elements[sending_rounds][sb] = a;
    sb += 1;
    //add to send buffer
}    



DATATYPE P_complete_Reveal(DATATYPE a)
{
DATATYPE result = a;
for(int t = 0; t < num_players-2; t++) // for other protocols, sending buffers may be different for each player
    result = XOR(result,receiving_args[t].received_elements[rounds-1][rb]);
rb+=1;
return result;
}
