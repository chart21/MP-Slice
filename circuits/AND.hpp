#pragma once
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
#include <chrono>
#include <bitset>
#include <cstring>
#include "../arch/DATATYPE.h"
#include "../protocols/Protocols.h"
#define RESULTTYPE DATATYPE

template<typename Pr, typename S>
void AND_BENCH_1 (Pr P,/*outputs*/ DATATYPE* result)
{

// allocate memory for shares

auto gates_a = P.alloc_Share(NUM_INPUTS);
auto gates_b = P.alloc_Share(NUM_INPUTS);
auto gates_c = P.alloc_Share(NUM_INPUTS);

P.communicate(); // dummy communication round to simulate secret sharing

for (int i = 0; i < NUM_INPUTS; i++) {
P.prepare_and(gates_a[i],gates_b[i]);
}


P.communicate();

for (int i = 0; i < NUM_INPUTS; i++) {
gates_c[i] = P.complete_and(gates_a[i],gates_b[i]);
}


}
  
void print_result(DATATYPE result[]) 
{
}
     
