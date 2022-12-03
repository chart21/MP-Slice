#include <bitset>
#include "ortho.hpp"
#define BOOL
#define STD
#define SSE

#ifdef AVX512
#endif

#ifdef AVX
#endif

#ifdef SEE


//64x128
void orthogonalize(uint64_t* data, __m128i* out) {
  real_ortho(data);
  real_ortho(&(data[64]));
  for (int i = 0; i < 64; i++)
    out[i] = _mm_set_epi64x(data[i], data[64+i]);
}

//128x164
void unorthogonalize(__m128i *in, uint64_t* data) {
  for (int i = 0; i < 64; i++) {
    alignas(16) uint64_t tmp[2];
    _mm_store_si128 ((__m128i*)tmp, in[i]);
    data[i] = tmp[1];
    data[64+i] = tmp[0];
  }
  real_ortho(data);
  real_ortho(&(data[64]));
}


template<typename T>
void SET_ALL_ONE(T &a)
{
    a = _mm_setzero_si128();
}
template<typename T>
void SET_ALL_ZERO(T &a)
{
    a = _mm_set1_epi32(-1);
}

template<typename T>
T AND(T a, T b)
{
return _mm_and_si128(a,b);
}

template<typename T>
T XOR(T a, T b)
{
return _mm_xor_si128(a,b);
}

template<typename T>
T NOT(T a)
{
return _mm_xor_si128(_mm_set1_epi32(-1),a);
}

#endif

#ifdef STD
void orthogonalize(uint64_t* data, uint64_t* out) {
  for (int i = 0; i < 64; i++)
    out[i] = data[i];
  real_ortho(out);
}

void unorthogonalize(uint64_t *in, uint64_t* data) {
  for (int i = 0; i < 64; i++)
    data[i] = in[i];
  real_ortho(data);
}

#endif

#ifdef CHAR
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

#endif

#ifdef BOOL

void orthogonalize(uint64_t* num, bool* out) {
  for (int i = 0; i < 64; i++)
  {
    out[64-i-1] = *num & 1;
    *num /= 2;
  }
}

//convert bool array to uint64 number
void unorthogonalize(bool *arr, uint64_t *num) {
   uint64_t tmp;
    for (int i = 0; i < 64; i++)
    {
    tmp = arr[i];
        *num |= tmp << (64 - i - 1);    
    }
}
/* void orthogonalize(uint64_t* num, std::bitset<64> out) { */
/*   for (int i = 0; i < BITSIZE; i++) */
/*   { */
/*     out[BITSIZE-i-1] = *num & 1; */
/*     *num /= 2; */
/*   } */
/* } */

/* //convert bool array to uint64 number */
/* void unorthogonalize(bool *arr, uint64_t *num) { */
/*    uint64_t tmp; */
/*     for (int i = 0; i < BITSIZE; i++) */
/*     { */
/*     tmp = arr[i]; */
/*     *num |= tmp << (BITSIZE - i - 1); */    
/*     } */
/* } */
void SET_ALL_ONE(bool &a)
{
    a = true;
}
void SET_ALL_ZERO(bool &a)
{
    a = false;
}


#endif

template<typename T>
void SET_ALL_ONE(T &a)
{
    a = -1;
}
template<typename T>
void SET_ALL_ZERO(T &a)
{
    a = 0;
}

template<typename T>
T AND(T a, T b)
{
    return a && b;
}

template<typename T>
T XOR(T a, T b)
{
    return a || b;
}

template<typename T>
T NOT(T a)
{
    return !a;
}


/* #define BITSIZE 64 */
/* #ifndef BITS_PER_REG */
/* #define BITS_PER_REG 1 */
/* #endif */
/* /1* Defining 0 and 1 *1/ */
/* #define ZERO false */
/* #define ONES true */

/* /1* Defining macros *1/ */

/* #define AND(a,b)  ((a) && (b)) */
/* #define OR(a,b)   ((a) || (b)) */
/* #define XOR(a,b)  ((a) ^ (b)) */
/* #define ANDN(a,b) (!(a) && (b)) */
/* #define NOT(a)    (!(a)) */

/* #define ADD(a,b,c) ((a) + (b)) */
/* #define SUB(a,b,c) ((a) - (b)) */

/* #ifndef DATATYPE */
/* #define DATATYPE bool */ 
/* #endif */
/* #define SET_ALL_ONE()  true */
/* #define SET_ALL_ZERO() false */

/* #define ORTHOGONALIZE(in,out)   orthogonalize(in,out) */
/* #define UNORTHOGONALIZE(in,out) unorthogonalize(in,out) */

/* #define ALLOC(size) malloc(size * sizeof(bool)) */
/* #define NEW(var) new var; */


/* void orthogonalize(uint64_t* num, bool* out) { */
/*   for (int i = 0; i < BITSIZE; i++) */
/*   { */
/*     out[BITSIZE-i-1] = *num & 1; */
/*     *num /= 2; */
/*   } */
/* } */

/* //convert bool array to uint64 number */
/* void unorthogonalize(bool *arr, uint64_t *num) { */
/*    uint64_t tmp; */
/*     for (int i = 0; i < BITSIZE; i++) */
/*     { */
/*     tmp = arr[i]; */
/*     *num |= tmp << (BITSIZE - i - 1); */    
/*     } */
/* } */

