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
sending_args[1].sent_elements[sending_rounds][sb] = a;
sb+=1;
return a;
}

Share P_receive_share_SRNG(int player)
{
Share dummy;
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
return;
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
DATATYPE result;
result = receiving_args[num_players-1].received_elements[rounds-1][rb]; 
rb+=1;
return result;
}
