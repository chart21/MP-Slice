#include <iostream>
#include <chrono>
/* #define DATTYPE 128 */
#include "arch/DATATYPE.h"
#include "arch/AES.h"
#include "arch/AES_BS_SHORT.h"
/* #include "arch/AES_BS.h" */
#include "circuits/xorshift.h"
#include "arch/SHA_256.h"
#include "config.h"
#ifdef __SHA__
#include "arch/SHA_256_x86.h"
#endif
int main() {

#ifdef __AES__
    std::cout << "AES-NI is supported" << std::endl;
#endif

#ifdef __VAES__
    std::cout << "VAES is supported" << std::endl;
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
std::chrono::system_clock::time_point finish5;
std::chrono::system_clock::time_point finish6;
std::chrono::system_clock::time_point finish7;
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

for (int i = 0; i < 1000; i++) {
    sha256_process(state, message, sizeof(message));
}
 
#ifdef __SHA__
for (int i = 0; i < 1000; i++) {
    sha256_process_x86(state, message, sizeof(message));
}
#endif

start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000000; i++) {
    for (int j = 0; j < 128; j++) {
        DO_ENC_BLOCK(m, k);
        k[0] = m;
        m = m+1;
    }
    }
 finish = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; i++) {
        AES__(plain__, key__, cipher__);
        plain__[0] = cipher__[0];
    }
 finish2 = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 1000000; i++) {
    for (int j = 0; j < 2; j++) 
        xor_shift(seed);
    }

 finish3 = std::chrono::high_resolution_clock::now();
    
for (int i = 0; i < 1000; i++) {
    sha256_process(state, message, sizeof(message));
}
 finish4 = std::chrono::high_resolution_clock::now();
 
#ifdef __SHA__
for (int i = 0; i < 1000; i++) {
    sha256_process_x86(state, message, sizeof(message));
}
 finish5 = std::chrono::high_resolution_clock::now();
std::cout << "SHA256 (Intrinsics): " << std::chrono::duration_cast<std::chrono::milliseconds>(finish5 - finish4).count() << std::endl;
std::cout << "SHA256 (Intrinsics) Throughput: " << 1.024 / std::chrono::duration_cast<std::chrono::milliseconds>(finish5 - finish4).count() * 1000 << "GB/s"<< std::endl;
#endif

auto a = new uint64_t[1000000000];
auto b = new uint64_t[1000000000];
auto c= new uint64_t[1000000000];
finish6 = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 1000000000; i++) {
c[i] = a[i] * b[i];
}
finish7 = std::chrono::high_resolution_clock::now();

std::cout << "AES_NI: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << std::endl;
std::cout << "AES_NI Throughput: " << 16.384*DATTYPE/128 / std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() * 1000 << "GB/s"<< std::endl;
std::cout << "AES_BS: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - finish).count() << std::endl;
std::cout << "AES_BS Throughput: " << 16.384*DATTYPE/128 / std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - finish).count()* 1000 << "GB/s"<< std::endl;
std::cout << "XOR_Shift: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish3 - finish2).count() << std::endl;
std::cout << "XOR_Shift Throughput: " << 16.384*DATTYPE/128 / std::chrono::duration_cast<std::chrono::milliseconds>(finish3 - finish2).count() * 1000 << "GB/s"<< std::endl;
std::cout << "SHA256: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish4 - finish3).count() * 1000 << std::endl;
std::cout << "SHA256 Throughput: " << 1.024 / std::chrono::duration_cast<std::chrono::milliseconds>(finish4 - finish3).count() * 1000 << "GB/s"<< std::endl;
std::cout << "Tested with 16384MB for AES, 1024MB for SHA256" << std::endl;

std::cout << "64-bit Mult: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish7 - finish6).count() << std::endl;

std::cout << m[0] << std::endl;
std::cout << plain__[0][0] << std::endl;
std::cout << seed << std::endl;
std::cout << state[0] << std::endl;
std::cout << message << std::endl;
std::cout << c[999999999] << std::endl;



    return 0;
}
