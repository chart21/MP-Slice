#pragma once
#include "../arch/DATATYPE.h"
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"

#include "../utils/randomizer.h"


/* struct Share{ */
/*     DATATYPE s; */
/* }; */
#define Share DATATYPE


Share P_share_SRNG(DATATYPE a)
{
DATATYPE s = XOR(a,getRandomVal(0));
sending_args[1].sent_elements[sending_rounds][sb] = s;
sb+=1;
return s;
}

Share P_receive_share_SRNG(int player)
{

DATATYPE share = receiving_args[player].received_elements[rounds-1][share_buffer[player]];
share_buffer[player] += 1;
return share;
}

Share P_xor(Share a, Share b)
{
    return XOR(a, b); 
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
DATATYPE ry = getRandomVal(0);
DATATYPE o1 = receiving_args[0].received_elements[rounds-1][rb];
DATATYPE o2 = receiving_args[0].received_elements[rounds-1][rb+1];
rb+=2;
*a = XOR(ry, XOR(AND(*a,o1),AND(*a,o2)));
sending_args[1].sent_elements[sending_rounds][sb] = *a; 
sb+=1;

}

// NAND both real Values to receive sharing of ~ (a&b) 
Share P_complete_and(Share a, Share b)
{
b = receiving_args[1].received_elements[rounds-1][rb];
rb+=1;
return XOR(a, b); 
}

void P_prepare_reveal_to_all(Share a)
{
sending_args[0].sent_elements[sending_rounds][sb] = a;
sb += 1;
    return;
}    


// These functions need to be somewhere else

DATATYPE P_complete_Reveal(Share a)
{
DATATYPE result;
result = XOR(a,receiving_args[0].received_elements[rounds-1][rb]); 
rb+=1;
return result;
}
