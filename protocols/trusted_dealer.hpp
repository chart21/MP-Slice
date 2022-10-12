#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
#include "../utils/printing.hpp"
#include "../utils/randomizer.h"


/* struct Share{ */
/*     DATATYPE s; */
/* }; */
#define Share DATATYPE


Share P_share_SRNG(DATATYPE a)
{
return a;
}

Share P_receive_share_SRNG(int player)
{
DATATYPE dummy;
return dummy;
}

Share P_xor(Share a, Share b)
{
    return a; 
}

Share P_xor_pub(Share a, DATATYPE b)
{
    return a; 
}

Share P_public_val(DATATYPE a)
{
    return a; 
}

Share P_not(Share a)
{
    return a;
}


//prepare AND -> send real value a&b to other P
void P_prepare_and(Share* a, Share* b)
{
DATATYPE randVals[num_players-1][3];
randVals[0][0] = getRandomVal(0);
randVals[0][1] = getRandomVal(0);
randVals[0][2] = SET_ALL_ZERO();
for(int t = 1; t < num_players-1; t++)
{
randVals[t][0] = getRandomVal(t);
randVals[t][1] = getRandomVal(t);
randVals[t][2] = getRandomVal(t);
}
DATATYPE a_sum = randVals[0][0];
DATATYPE b_sum = randVals[0][1];
DATATYPE c_sum = randVals[0][2];
for(int t = 1; t < num_players-1; t++)
{
    a_sum = XOR(a_sum,randVals[t][0]);
    b_sum = XOR(b_sum,randVals[t][1]);
    c_sum = XOR(c_sum,randVals[t][1]);
}
randVals[0][2] = XOR(AND(a_sum,b_sum),c_sum); 
//ensure a*b=c

sending_args[0].sent_elements[sending_rounds][sb] = randVals[0][2];
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share P_complete_and(Share a, Share b)
{
return a;
}

void P_prepare_reveal_to_all(Share a)
{
    return;
}    


// These functions need to be somewhere else

DATATYPE P_complete_Reveal(Share a)
{
return a;
}
