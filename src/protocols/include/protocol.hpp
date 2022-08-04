#ifndef protocol
#define protocol
#include <cstdint>

struct MP_Protocol{
   inline void xorGate(uint_fast64_t y_index, uint_fast64_t z_index);
   inline void andGate(uint_fast64_t x_index, uint_fast64_t y_index, uint_fast64_t z_index);
   inline void notGate(uint_fast64_t x_index, uint_fast64_t y_index);
};
    //When a circuit is evaluated, the Protocol gets the input indicies (where to fetch the inputs) from and an output index(where to save the input to)
#endif
