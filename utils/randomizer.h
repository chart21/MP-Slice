#pragma once

#if RANDOM_ALGORITHM == 0
#include "../circuits/xorshift.h"
#elif RANDOM_ALGORITHM == 1
#include "../arch/AES_BS_SHORT.h"
#elif RANDOM_ALGORITHM == 2
#include "../arch/AES.h"
#endif
#include "../networking/buffers.h"
#include <stdint.h>



DATATYPE getRandomVal(int link_id)
{
#if RANDOM_ALGORITHM == 0
   if(num_generated[link_id] > 63)
   {
       num_generated[link_id] = 0;
       xor_shift(srng[link_id]);
   }
   num_generated[link_id] += 1;
   return srng[link_id][num_generated[link_id] -1];
#elif RANDOM_ALGORITHM == 1
    if(num_generated[link_id] > 127)
    {
        num_generated[link_id] = 0;
        AES__(counter[link_id], key[link_id], cipher[link_id]);
        for (int i = 0; i < 128; i++) {
           counter[link_id][i] += 1;
        }
    }
    num_generated[link_id] += 1;
    return cipher[link_id][num_generated[link_id] -1];
#elif RANDOM_ALGORITHM == 2
    #if DATTYPE >= 128
    DO_ENC_BLOCK(counter[link_id], key[link_id]);
    counter = counter + 1;
    return counter[link_id];
    #else
     #define BUFFER_SIZE 128/DATTYPE
    if(num_generated[link_id] > BUFFER_SIZE - 1)
    {
        num_generated[link_id] = 0;
        DO_ENC_BLOCK(counter[link_id], key[link_id]);
    }
    num_generated[link_id] += 1;
    return ((DATATYPE*) counter[link_id])[num_generated[link_id] -1];
    #endif
#endif
}

/* DATATYPE getRandomVal(int link_id) */
/* { */
/*    if(num_generated[link_id] == BITLENGTH -1) */
/*    { */
/*        num_generated[link_id] = 0; */
/*        xor_shift(srng[link_id]); */
/*    } */
/*    num_generated[link_id] += 1; */
/*    return srng[link_id][num_generated[link_id] -1]; */
/* } */

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
