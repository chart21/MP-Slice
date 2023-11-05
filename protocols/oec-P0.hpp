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
return getRandomVal(2);
}

Share P_receive_share_SRNG(int player)
{
return getRandomVal(player);
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
DATATYPE rl = getRandomVal(1);
DATATYPE rr = getRandomVal(1);

DATATYPE o1 = XOR(*a,rr);
DATATYPE o2 = XOR(*b,rl);
sending_args[1].sent_elements[sending_rounds][sb] = o1; //P0 should only have a link to P3
sending_args[1].sent_elements[sending_rounds][sb+1] = o2; //P0 should only have a link to P3
sb+=2;
*a = AND(*a,rl);
*b = XOR(AND(*b,rr),AND(rl,rr));

}

// NAND both real Values to receive sharing of ~ (a&b) 
Share P_complete_and(Share a, Share b)
{
DATATYPE rx = getRandomVal(0);
DATATYPE ry = getRandomVal(1);
return XOR(XOR(a, b),XOR(rx,ry));
}

void P_prepare_reveal_to_all(Share a)
{
    sending_args[0].sent_elements[sending_rounds][sb] = a;
    sending_args[1].sent_elements[sending_rounds][sb] = a;
    sb += 1;
    //add to send buffer
}    


// These functions need to be somewhere else

DATATYPE P_complete_Reveal(Share a)
{
DATATYPE result;
result = XOR(a,receiving_args[1].received_elements[rounds-1][rb]); 
rb+=1;
return result;
}
