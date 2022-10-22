#pragma once
#include "../circuits/xorshift.h"
#include "../networking/buffers.h"
#include <stdint.h>
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

uint64_t shuffle_table[4];
// The actual algorithm
uint64_t next(void)
{
    uint64_t s1 = shuffle_table[0];
    uint64_t s0 = shuffle_table[1];
    uint64_t result = s0 + s1;
    shuffle_table[0] = s0;
    s1 ^= s1 << 23;
    shuffle_table[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return result;
}
