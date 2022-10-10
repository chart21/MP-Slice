#pragma once
#include "../circuits/xorshift.h"
#include "../networking/buffers.h"

DATATYPE getRandomVal(int link_id)
{
   if(num_generated[link_id] == BITLENGTH -1)
   {
       num_generated[link_id] = 0;
       xor_shift(srng[link_id]);
   }
   num_generated[link_id] += 1;
   return srng[link_id][num_generated[link_id] -1];
}

void getRandomVal_BITS_PER_REG(DATATYPE* a, int link_id)
{
       xor_shift(srng[link_id]);
       for (int i = 0; i < BITS_PER_REG; i++) {
           a[i] = srng[link_id][i];
       }
}

