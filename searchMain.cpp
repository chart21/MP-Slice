#include <cstdint>
#include <iostream>
#include <cstring>
#include <random>
#include <bitset>
#include "arch/DATATYPE.h"
#include "circuits/searchBitSlice.c"
#include "circuits/xorshift.c"

void print_num(DATATYPE var) 
{
    uint8_t v8val[sizeof(DATATYPE)];
    std::memcpy(v8val, &var, sizeof(v8val));
    for (uint i = sizeof(DATATYPE); i > 0; i--)
        std::cout << std::bitset<sizeof(uint8_t)*8>(v8val[i-1]) << std::endl;
        //std::cout << v8val[i]<< std::endl;
}

void insertManually(DATATYPE dataset[n][BITLENGTH], DATATYPE elements[n], uint64_t origData[n][BITS_PER_REG], uint64_t origElements[], int c, int b, uint64_t numElement, uint64_t numDataset ){

unorthogonalize(elements, origElements);

for (int i = 0; i < n; i++) {
 unorthogonalize(dataset[i], origData[i]);   
}
origData[c][b] = numDataset;
origElements[b] = numElement;
std::cout << origData[c][b] << origElements[b] << std::endl;
orthogonalize(origElements, elements);

for (int i = 0; i < n; i++) {
 orthogonalize(origData[i], dataset[i]);   
}
}
void randomizeInputs(DATATYPE dataset[n][BITLENGTH], DATATYPE elements[n])
{
    // init randomization
uint64_t* iseed = new uint64_t[BITS_PER_REG];
for (int i = 0; i < BITS_PER_REG; i++) {
    iseed[i] = rand();
}
DATATYPE* seed = new DATATYPE[BITLENGTH];
orthogonalize(iseed, seed);

//generate random data
for (int i = 0; i < n; i++) {
     xor_shift__(seed, dataset[i]);
 }
    xor_shift__(seed, elements);
}


int main(int argc, char *argv[])
{

/// Processing Inputs ///

uint64_t (*origData)[BITS_PER_REG] = new uint64_t[n][BITS_PER_REG];
uint64_t* origElements = new uint64_t[BITS_PER_REG];


// read inputs ///


//Slicing inputs
//DATATYPE (*dataset)[BITLENGTH] = malloc(sizeof(DATATYPE[n][BITLENGTH]));
DATATYPE (*dataset)[BITLENGTH] = new DATATYPE[n][BITLENGTH];
DATATYPE* elements = new DATATYPE[BITLENGTH];
for (int i = 0; i < n; i++) {
 orthogonalize(origData[i], dataset[i]);   
}
orthogonalize(origElements, elements);

// generate random input data instead of reading from file
randomizeInputs(dataset, elements);



//modify certain data to test functionality
insertManually(dataset, elements, origData, origElements, 0,0 , 200, 200);


DATATYPE* found = new DATATYPE;
search__(dataset, elements, found);

print_num(*found);
}

