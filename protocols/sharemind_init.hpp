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
#define Share DATATYPE

void send_and_receive()
{
for (int t = 0; t < num_players-1; t++)
{
if(sending_args[t].elements_to_send[rounds] != 0)
{
    sending_args[t].total_rounds += 1;
}
rounds++;
if(receiving_args[t].elements_to_rec[rounds-1] != 0)
{
    sending_args[t].total_rounds += 1;
}


}
}
DATATYPE P_share_SRNG(DATATYPE a)
{
    return a;
}

Share P_receive_share_SRNG(int player)
{
Share dummy;
return dummy;
}

DATATYPE P_share(DATATYPE a)
{
sending_args[pprev].elements_to_send[rounds] += 1;
sending_args[pnext].elements_to_send[rounds] +=1;
sb+=1;
DATATYPE dummy;
return dummy;
      }





void P_share(DATATYPE a[], int length)
{
    for(int l = 0; l < length; l++)
        a[l] = P_share(a[l]);
                                           //
}
Share P_public_val(DATATYPE a)
{
    return a;
}

DATATYPE P_not(DATATYPE a)
{
   return a;
}

// Receive sharing of ~XOR(a,b) locally
DATATYPE P_xor(DATATYPE a, DATATYPE b)
{
   return a;
}



void reshare(DATATYPE a, DATATYPE u[])
{
 
}
//prepare AND -> send real value a&b to other P
void P_prepare_and(DATATYPE* a, DATATYPE* b)
{
sending_args[pnext].elements_to_send[rounds] += 1;
sending_args[pprev].sent_elements[sending_rounds][sb] += 1;
sb+=1;
//return u[player_id] * v[player_id];
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE P_complete_and(DATATYPE a, DATATYPE b)
{
receiving_args[pprev].elements_to_rec[rounds-1] += 1;
receiving_args[pnext].elements_to_rec[rounds-1] += 1;
DATATYPE dummy;
return dummy;
}

void P_prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < num_players-1; t++) 
        sending_args[t].sent_elements[rounds] += 1;
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

DATATYPE P_complete_Reveal(DATATYPE a)
{
for(int t = 0; t < num_players-1; t++) 
    receiving_args[t].elements_to_rec[rounds-1]+=1;
rb+=1;
return a;
}
