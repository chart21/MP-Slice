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
DATATYPE value = receiving_args[player].received_elements[rounds-1][share_buffer[player]];
share_buffer[player] += 1;
return value;
}

Share P_xor(Share a, Share b)
{
    return XOR(a, b); 
}

Share P_xor_pub(Share a, DATATYPE b)
{
    return XOR(a,b); 
}

Share P_public_val(DATATYPE a)
{
    return a; 
}

Share P_not(Share a)
{
    return NOT(a);
}


//prepare AND -> send real value a&b to other P
void P_prepare_and(Share* a, Share* b)
{
return;
}

// NAND both real Values to receive sharing of ~ (a&b) 
Share P_complete_and(Share a, Share b)
{
return AND(a,b);
}

void P_prepare_reveal_to_all(Share a)
{
    for(int t = 0; t < num_players-1; t++)
    {
        sending_args[t].sent_elements[sending_rounds][sb] = a;
    //add to send buffer
    }    
    sb += 1;
    //add to send buffer
}    


// These functions need to be somewhere else

DATATYPE P_complete_Reveal(Share a)
{
return a;
}
