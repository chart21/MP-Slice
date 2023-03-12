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
#define COMMUNICATION_ROUNDS 1000

template<typename Pr, typename S>
void AND_BENCH_COMMUNICATION_ROUNDS (Pr P,/*outputs*/ DATATYPE* result)
{
int loop_num = NUM_INPUTS/COMMUNICATION_ROUNDS;
// allocate memory for shares

auto gates_a = P.alloc_Share(NUM_INPUTS);
auto gates_b = P.alloc_Share(NUM_INPUTS);
auto gates_c = P.alloc_Share(NUM_INPUTS);
P.communicate(); // dummy communication round to simulate secret sharing

for(int j = 0; j < COMMUNICATION_ROUNDS; j++) {

for (int i = 0; i < loop_num; i++) {
P.prepare_and(gates_a[i+j*COMMUNICATION_ROUNDS],gates_b[i+j*COMMUNICATION_ROUNDS]);
}


P.communicate();

for (int i = 0; i < loop_num; i++) {
gates_c[i+j*COMMUNICATION_ROUNDS] = P.complete_and(gates_a[i+j*COMMUNICATION_ROUNDS],gates_b[i+j*COMMUNICATION_ROUNDS]);
}

}
}



  
void print_result(DATATYPE result[]) 
{
}
     
