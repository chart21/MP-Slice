#pragma once
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <random>
#include <bitset>
#include <new>
#include <memory>
#include "arch/DATATYPE.h"
/* #include "circuits/searchBitSlice.c" */
#include "circuits/debug_template.cpp"
#include "circuits/xorshift.h"

#include "config.h"
#include "protocols/Protocols.h"

#include "utils/randomizer.h"
#include "utils/timing.hpp"

/* #include "networking/client_macro.hpp" */
/* #include "networking/server_macro.hpp" */
/* #include "networking/client.hpp" */
/* #include "networking/server.hpp" */
#include "networking/sockethelper.h"
#include "networking/buffers.h"




void print_bool(uint8_t* found)
{
     for (int j = 0; j < BITS_PER_REG; j++)
         std::cout << +found[j];
    std::cout << '\n';
}


void search_Compare(uint64_t origData[NUM_INPUTS][BITS_PER_REG], uint64_t origElements[], uint8_t* found)
{
 for (int i = 0; i < NUM_INPUTS; i++) {
    for (int j = 0; j < BITS_PER_REG; j++) {
        if(origData[i][j] == origElements[j])
            found[j] = 1;
    }   
}
}


void insertManually(DATATYPE dataset[NUM_INPUTS][BITLENGTH], DATATYPE elements[NUM_INPUTS], uint64_t origData[NUM_INPUTS][BITS_PER_REG], uint64_t origElements[], int c, int b, uint64_t numElement, uint64_t numDataset ){

/* unorthogonalize(elements, origElements); */

/* for (int i = 0; i < NUM_INPUTS; i++) { */
/*  unorthogonalize(dataset[i], origData[i]); */   
/* } */
origData[c][b] = numDataset;
origElements[b] = numElement;
/* std::cout << origData[c][b] << origElements[b] << std::endl; */


uint8_t* cfound = new uint8_t[BITS_PER_REG]{0};
/* funcTime("Plain search", search_Compare, origData, origElements, cfound); */

#if PRINT == 1
search_Compare(origData, origElements, cfound);
print_bool(cfound);
#endif



orthogonalize(origElements, elements);

for (int i = 0; i < NUM_INPUTS; i++) {
 orthogonalize(origData[i], dataset[i]);   
}
}
void randomizeInputs(DATATYPE dataset[NUM_INPUTS][BITLENGTH], DATATYPE elements[NUM_INPUTS])
{
    // init randomization
uint64_t* iseed = NEW(uint64_t[BITS_PER_REG]);
for (int i = 0; i < BITS_PER_REG; i++) {
    iseed[i] = rand();
}
DATATYPE* seed = new DATATYPE[BITLENGTH];
orthogonalize(iseed, seed);

//generate random data
for (int i = 0; i < int( NUM_INPUTS / (sizeof(DATATYPE)*8)/BITS_PER_REG ); i++) {
     xor_shift__(seed, dataset[i]);
 }
xor_shift__(seed, elements);
 /* funcTime("xor_shift",xor_shift__,seed, elements); */
}

//init 1 srng per link

void init_comm()
{


// sharing
input_length[0] = BITLENGTH * NUM_INPUTS;
input_length[1] = BITLENGTH;

// revealing
reveal_length[0] = 1;
reveal_length[1] = 1;
reveal_length[2] = 1;

total_comm = 2 - use_srng_for_inputs;

// function

for (int k = BITLENGTH >> 1; k > 0; k = k >> 1)
{
    total_comm = total_comm + 1;
}
elements_per_round = new int[total_comm];



//function

int i = 1 - use_srng_for_inputs;
for (int k = BITLENGTH >> 1; k > 0; k = k >> 1)
{
    elements_per_round[i] = k*NUM_INPUTS;
    i+=1;
}




}
#if FUNCTION_IDENTIFIER == 7
void generateElements()
{
auto inputs = new DATATYPE[num_players][BITLENGTH];
for(int i = 0; i < num_players; i++)
{
for(int j = 0; j < BITLENGTH; j++)
{

#if PARTY == 0
inputs[i][j] = 11;
#elif PARTY == 2
inputs[i][j] = 7;
#elif PARTY == 1
inputs[i][j] = 5;
#else
inputs[i][j] = 3;
#endif
}
}

player_input = (DATATYPE*) inputs[PARTY];

}
#else
void generateElements()
{
auto inputs = new DATATYPE[num_players][BITLENGTH];
for(int i = 0; i < num_players; i++)
{
for(int j = 0; j < BITLENGTH; j++)
{
inputs[i][j] = SET_ALL_ONE();
if(j == i)
    inputs[i][j] = SET_ALL_ZERO();
}
}

player_input = (DATATYPE*) inputs[PARTY];

}
#endif
