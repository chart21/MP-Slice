#include "arch/SSE.h"
#include <iostream>
#include <chrono>
#define DATTYPE 128
#include "arch/DATATYPE.h"
#include "arch/AES.h"
#include "arch/AES_BS_SHORT.h"
#include "circuits/xorshift.h"

int main() {

    /* std::cout << aes128_self_test() << std::endl; */
    DATATYPE plain__[128];
    DATATYPE key__[11][128];
    DATATYPE cipher__[128];
    DATATYPE seed[64];

    DATATYPE m;
    DATATYPE k[11];

std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point finish;
std::chrono::system_clock::time_point finish2;
std::chrono::system_clock::time_point finish3;


start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 128; j++) {
        DO_ENC_BLOCK(m, k);
        k[0] = m;
        m = m+1;
    }
    }
 finish = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++) {
        AES__(plain__, key__, cipher__);
        plain__[0] = cipher__[0];
    }
 finish2 = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 2; j++) 
        xor_shift(seed);
    }

 finish3 = std::chrono::high_resolution_clock::now();
    

std::cout << "AES_NI: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
std::cout << "AES_BS: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish2 - finish).count() << std::endl;
std::cout << "XOR_Shift: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish3 - finish2).count() << std::endl;
std::cout << m[0] << std::endl;
std::cout << plain__[0][0] << std::endl;
std::cout << seed << std::endl;


    return 0;
}
