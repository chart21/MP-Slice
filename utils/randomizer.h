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

#if RANDOM_ALGORITHM == 2
    #if DATTYPE <= 64
#include <x86intrin.h>
#include <immintrin.h>
#include <memory>
    #ifdef __AVX512__
#define MM_XOR _mm512_xor_si512
#define MM_AES_ENC _mm512_aesenc_epi128
#define MM_AES_DEC _mm512_aesdec_epi128
#define MM_AES_ENC_LAST _mm512_aesenclast_epi128
#define MM_AES_DEC_LAST _mm512_aesdeclast_epi128
#define COUNT_TYPE __m512i
#define load _mm512_load_epi64
    #elif defined __AVX2__
#define MM_XOR _mm256_xor_si256
#define MM_AES_ENC _mm256_aesenc_epi128
#define MM_AES_DEC _mm256_aesdec_epi128
#define MM_AES_ENC_LAST _mm256_aesenclast_epi128
#define MM_AES_DEC_LAST _mm256_aesdeclast_epi128
#define COUNT_TYPE __m256i
#define load _mm256_load_epi64
    #elif defined __SSE2__
#define MM_XOR _mm_xor_si128
#define MM_AES_ENC _mm_aesenc_si128
#define MM_AES_DEC _mm_aesdec_si128
#define MM_AES_ENC_LAST _mm_aesenclast_si128
#define MM_AES_DEC_LAST _mm_aesdeclast_si128
#define COUNT_TYPE __m128i
    #endif
COUNT_TYPE key[num_players][11]{0};
    #endif
#endif


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
    counter[link_id] += 1;
    return counter[link_id];
    #else
        int bufferlimit = BUFFER_SIZE - 1;
    
    if(num_generated[link_id] > bufferlimit - 1)
    {
        num_generated[link_id] = 0;
        COUNT_TYPE vectorized_counter{0};
        for (int i = 0; i < BUFFER_SIZE; i++) {
            counter[link_id][i] += 1+BUFFER_SIZE;
        }
        memcpy(&vectorized_counter, counter[link_id], sizeof(COUNT_TYPE));
        DO_ENC_BLOCK(vectorized_counter, key[link_id]);
    }
    num_generated[link_id] += 1;
    return counter[link_id][num_generated[link_id] -1];
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

/* #if RANDOM_ALGORITHM == 0 */
/* void getRandomVal_BITS_PER_REG(DATATYPE* a, int link_id) */
/* { */
/*        xor_shift(srng[link_id]); */
/*        for (int i = 0; i < BITS_PER_REG; i++) { */
/*            a[i] = srng[link_id][i]; */
/*        } */
/* } */
/* #endif */
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
