
/* ******************************************** *\
 *
 *
 *  DATATYPE: the base type of every value.
 *  SDATATYPE: the signed version of DATATYPE.
\* ******************************************** */


/* Including headers */
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef CHAR
#define CHAR
#endif

#ifndef BITS_PER_REG
#define BITS_PER_REG 8
#endif
#ifndef LOG2_BITS_PER_REG
#define LOG2_BITS_PER_REG 1
#endif

/* Defining 0 and 1 */
#define ZERO 0
#define ONES -1

/* Defining macros */
#define REG_SIZE BITS_PER_REG
#define CHUNK_SIZE 64

#define AND(a,b)  ((a) & (b))
#define OR(a,b)   ((a) | (b))
#define XOR(a,b)  ((a) ^ (b))
#define ANDN(a,b) (~(a) & (b))
#define NOT(a)    (~(a))

/* #define ADD(a,b,c) ((a) + (b)) */
/* #define SUB(a,b,c) ((a) - (b)) */

#define MUL_SIGNED(a,b,c) a * b
#define ADD_SIGNED(a,b,c) a + b
#define SUB_SIGNED(a,b,c) a - b



#ifndef DATATYPE
#if BITS_PER_REG == 4
#define DATATYPE uint8_t // TODO: use something else? do something else?
                         // (needed for Photon right now)
#define SDATATYPE int8_t
#elif BITS_PER_REG == 8
#define DATATYPE uint8_t
#define SDATATYPE int8_t
#elif BITS_PER_REG == 16
#define DATATYPE uint16_t
#define SDATATYPE int16_t
#elif BITS_PER_REG == 32
#define DATATYPE uint32_t
#define SDATATYPE int32_t
#else
#define DATATYPE uint64_t
#define SDATATYPE int64_t
#endif
#endif

#define SET_ALL_ONE()  -1
#define SET_ALL_ZERO() 0

#define ORTHOGONALIZE(in,out)   orthogonalize(in,out)
#define UNORTHOGONALIZE(in,out) unorthogonalize(in,out)

#define ALLOC(size) malloc(size * sizeof(uint8_t))
#define NEW(var) new var;


#ifdef RUNTIME


/* Orthogonalization stuffs */
static uint64_t mask_l[6] = {
	0xaaaaaaaaaaaaaaaaUL,
	0xccccccccccccccccUL,
	0xf0f0f0f0f0f0f0f0UL,
	0xff00ff00ff00ff00UL,
	0xffff0000ffff0000UL,
	0xffffffff00000000UL
};

static uint64_t mask_r[6] = {
	0x5555555555555555UL,
	0x3333333333333333UL,
	0x0f0f0f0f0f0f0f0fUL,
	0x00ff00ff00ff00ffUL,
	0x0000ffff0000ffffUL,
	0x00000000ffffffffUL
};


void real_ortho(uint64_t data[]) {
  for (int i = 0; i < 3; i ++) {
    int n = (1UL << i);
    for (int j = 0; j < 8; j += (2 * n))
      for (int k = 0; k < n; k ++) {
        uint64_t u = data[j + k] & mask_l[i];
        uint64_t v = data[j + k] & mask_r[i];
        uint64_t x = data[j + n + k] & mask_l[i];
        uint64_t y = data[j + n + k] & mask_r[i];
        data[j + k] = u | (x >> n);
        data[j + n + k] = (v << n) | y;
      }
  }
}

#ifdef ORTHO

void orthogonalize(uint64_t* data, uint8_t* out) {
  /* for (int i = 0; i < 8; i++) */
  /*   out[i] = data[i]; */
  real_ortho(data);
  for (int i = 0; i < 64; i++)
    out[i] = ((uint8_t*) data)[i];
  
}

void unorthogonalize(uint8_t *in, uint64_t* data) {
  for (int i = 0; i < 8; i++)
    data[i] = ((uint64_t*)in)[i];
  real_ortho(data);
}

#else

void orthogonalize(uint64_t* data, uint64_t* out) {
  for (int i = 0; i < 64; i++)
    out[i] = data[i];
}

void unorthogonalize(uint64_t *in, uint64_t* data) {
  for (int i = 0; i < 64; i++)
    data[i] = in[i];
}


#endif /* ORTHO */

#endif /* NO_RUNTIME */
