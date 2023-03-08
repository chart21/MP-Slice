#include <iostream>
#include <chrono>
/* #define DATTYPE 128 */
#include "arch/DATATYPE.h"
#include "arch/AES.h"
#include "arch/AES_BS_SHORT.h"
/* #include "arch/AES_BS.h" */
#include "circuits/xorshift.h"
#include "arch/SHA_256.h"

int main() {

#ifdef __AES__
    std::cout << "AES-NI is supported" << std::endl;
#endif

#ifdef __SSE4_1__
    std::cout << "SSE4.1 is supported" << std::endl;
#endif

#ifdef __SSE4_2__
    std::cout << "SSE4.2 is supported" << std::endl;
#endif

#ifdef __AVX__
    std::cout << "AVX is supported" << std::endl;
#endif

#ifdef __AVX2__
    std::cout << "AVX2 is supported" << std::endl;
#endif

#ifdef __AVX512F__
    std::cout << "AVX512F is supported" << std::endl;
#endif

#ifdef __AVX512BW__
    std::cout << "AVX512BW is supported" << std::endl;
#endif

#ifdef __SHA__
    std::cout << "SHA is supported" << std::endl;
#endif

#ifdef __PEXT__
    std::cout << "PEXT is supported" << std::endl;
#endif


    /* std::cout << aes128_self_test() << std::endl; */
    DATATYPE plain__[128];
    DATATYPE key__[11][128];
    DATATYPE cipher__[128];
    DATATYPE seed[64];

    DATATYPE m;
    DATATYPE k[11];

    uint8_t message[128000];
    /* memset(message, 0x00, sizeof(message)); */
    message[0] = 0x80;

    /* initial state */
    uint32_t state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };



std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point finish;
std::chrono::system_clock::time_point finish2;
std::chrono::system_clock::time_point finish3;
std::chrono::system_clock::time_point finish4;

// Warmup
    for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 128; j++) {
        DO_ENC_BLOCK(m, k);
        k[0] = m;
        m = m+1;
    }
    }
    for (int i = 0; i < 1000; i++) {
        AES__(plain__, key__, cipher__);
        plain__[0] = cipher__[0];
    }
for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 2; j++) 
        xor_shift(seed);
    }



start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < 128; j++) {
        DO_ENC_BLOCK(m, k);
        k[0] = m;
        m = m+1;
    }
    }
 finish = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        AES__(plain__, key__, cipher__);
        plain__[0] = cipher__[0];
    }
 finish2 = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < 2; j++) 
        xor_shift(seed);
    }

 finish3 = std::chrono::high_resolution_clock::now();
    
for (int i = 0; i < 1000; i++) {
    sha256_process(state, message, sizeof(message));
}
 finish4 = std::chrono::high_resolution_clock::now();

std::cout << "AES_NI: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << std::endl;
std::cout << "AES_BS: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - finish).count() << std::endl;
std::cout << "XOR_Shift: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish3 - finish2).count() << std::endl;
std::cout << "SHA256: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish4 - finish3).count() << std::endl;
std::cout << m[0] << std::endl;
std::cout << plain__[0][0] << std::endl;
std::cout << seed << std::endl;
std::cout << state[0] << std::endl;
std::cout << message << std::endl;




    return 0;
}
