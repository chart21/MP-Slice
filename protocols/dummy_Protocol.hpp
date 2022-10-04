#include "../arch/DATATYPE.h"


// Share of each player is ~a
void P_share(DATATYPE a, DATATYPE s[])
{
   for (int i = 0; i < num_players; i++) {
       s[i] = NOT(a);
   } 
}


// Receive sharing of ~XOR(a,b) locally
DATATYPE P_xor(DATATYPE a, DATATYPE b)
{
   return NOT(XOR(a, b));
}

//prepare AND -> send real value a&b to other P
DATATYPE P_prepare_and(DATATYPE a, DATATYPE b)
{
  return AND(NOT(a),NOT(b));
}

// NAND both real Values to receive sharing of ~ (a&b) 
DATATYPE P_receive_and(DATATYPE a, DATATYPE b)
{
 return NOT(AND(a,b));
}

void P_prepare_reveal(DATATYPE a)
{
    //add to send buffer
}    

// These functions need to be somewhere else

void P_communicate_Reveal()
{
    // send all reveals
}

void P_communicate_AND()
{
    // send all AND gates
}

void P_communicate_Shares()
{
    // send all reveals
}

