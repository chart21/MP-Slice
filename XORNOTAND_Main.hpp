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
#include "circuits/XORNOTAND.hpp"
#include "circuits/xorshift.h"

#include "config.h"
#include "protocols/Protocols.h"

#include "utils/randomizer.h"
#include "utils/timing.hpp"

#include "networking/sockethelper.h"
#include "networking/buffers.h"

#include "protocols/CircuitInfo.hpp"

#define FUNCTION XORNOTAND

void print_bool(uint8_t* found)
{
     for (int j = 0; j < BITS_PER_REG; j++)
         std::cout << +found[j];
    std::cout << '\n';
}


void compare(uint64_t origData[num_players][DATTYPE], uint64_t result[DATTYPE])
{
    for (int j = 0; j < DATTYPE; j++) {
        origData[0][j] = !( origData[0][j] ^ origData[1][j] );
#if num_players == 4
        origData[2][j] = !( origData[2][j] ^ origData[3][j] );
#endif
        origData[0][j] = origData[0][j] && origData[2][j];

        result[j] = origData[0][j];
    }   
}






void generateElements()
{


DATATYPE (*dataset)[BITLENGTH] = NEW(DATATYPE[num_players][BITLENGTH]);


for (int i = 0; i < num_players; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
        dataset[i][j] = SET_ALL_ONE();
    }
}

player_input = dataset[player_id];

alignas(DATTYPE) uint64_t regular_data[num_players][DATTYPE];
alignas(DATTYPE) uint64_t result[DATTYPE];
for (int i = 0; i < num_players; i++) {
UNORTHOGONALIZE(dataset[i], regular_data[i]);
    }
    //
print_result(dataset[0]);
compare(regular_data, result);
print_bool((uint8_t*)result);

}




