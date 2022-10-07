#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
// Share of each player is ~a
DATATYPE P_share(DATATYPE a)
{
DATATYPE s[num_players];
   for (int i = 0; i < num_players; i++) {
       s[i] = NOT(a);
       if(i < player_id){
            sending_args[i].sent_elements[sending_rounds][sb] = s[i];
       }
       else if (i > player_id) { //offset
            sending_args[i-1].sent_elements[sending_rounds][sb] = s[i];
       }
   }
   sb += 1;
   return s[player_id];
      }





// Receive sharing of ~XOR(a,b) locally
DATATYPE P_xor(DATATYPE a, DATATYPE b)
{
   return NOT(XOR(a, b));
}

//prepare AND -> send real value a&b to other P
DATATYPE P_prepare_and(DATATYPE a, DATATYPE b)
{
    for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
        sending_args[t].sent_elements[sending_rounds][sb] = AND(NOT(a),NOT(b));
    sb += 1;
  return AND(NOT(a),NOT(b));
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE P_complete_and(DATATYPE a)
{
DATATYPE result;
for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
    result = NOT(AND(a,receiving_args[t].received_elements[rounds-1][rb]));
rb+=1;
return result;
}

void P_prepare_reveal_to_all(DATATYPE a)
{
    for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
        sending_args[t].sent_elements[sending_rounds][sb] = a;
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

DATATYPE P_complete_Reveal(DATATYPE a)
{
DATATYPE result;
for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
    result = NOT(AND(a,receiving_args[t].received_elements[rounds-1][rb]));
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

