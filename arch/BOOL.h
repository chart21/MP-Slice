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
#include <stdbool.h>
#include "../Datastructures/bit_array_vec.hpp"
#ifndef BOOL
#define BOOL
#endif
#define BITSIZE 64
#ifndef BITS_PER_REG
#define BITS_PER_REG 1
#endif
/* Defining 0 and 1 */
#define ZERO false
#define ONES true

/* Defining macros */

#define AND(a,b)  ((a) && (b))
#define OR(a,b)   ((a) || (b))
#define XOR(a,b)  ((a) ^ (b))
#define ANDN(a,b) (!(a) && (b))
#define NOT(a)    (!(a))

#define ADD(a,b,c) ((a) + (b))
#define SUB(a,b,c) ((a) - (b))

#ifndef DATATYPE
#define DATATYPE bool 
#endif

#ifndef DATATYPE_REF
#define DATATYPE_REF std::vector<bool>::reference
/* #define DATATYPE_REF boost::dynamic_bitset<unsigned char>::reference */
#endif

#define SET_ALL_ONE()  true
#define SET_ALL_ZERO() false

#define ORTHOGONALIZE(in,out)   orthogonalize(in,out)
#define UNORTHOGONALIZE(in,out) unorthogonalize(in,out)

#define ALLOC(size) malloc(size * sizeof(bool))
#define NEW(var) new var;


void orthogonalize(uint64_t* num, bool* out) {
  for (int i = 0; i < BITSIZE; i++)
  {
    out[BITSIZE-i-1] = *num & 1;
    *num /= 2;
  }
}

//convert bool array to uint64 number
void unorthogonalize(bool *arr, uint64_t *num) {
   uint64_t tmp;
    for (int i = 0; i < BITSIZE; i++)
    {
    tmp = arr[i];
    *num |= tmp << (BITSIZE - i - 1);    
    }
}

