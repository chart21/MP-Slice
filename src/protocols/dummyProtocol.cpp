#include "include/protocol.hpp"
#include "include/network.hpp"


class DummyProtocol: public MP_Protocol{

    int* evaluation;
   
    DummyProtocol(int x) : array(new int[x]) {};

    inline void xorGate(uint_fast64_t y_index, uint_fast64_t z_index);
    inline void andGate(uint_fast64_t x_index, uint_fast64_t y_index, uint_fast64_t z_index);
    inline void notGate(uint_fast64_t x_index, uint_fast64_t y_index);
}
