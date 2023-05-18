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
#include "circuits/xorshift.h"

#include "config.h"
#include "protocols/Protocols.h"

#include "utils/randomizer.h"
#include "utils/timing.hpp"

#include "networking/sockethelper.h"
#include "networking/buffers.h"

#if FUNCTION_IDENTIFIER == 2 || FUNCTION_IDENTIFIER == 5 || FUNCTION_IDENTIFIER == 6
#define FUNCTION AND_BENCH_1
#include "circuits/AND.hpp"
#else
#define FUNCTION AND_BENCH_COMMUNICATION_ROUNDS
#include "circuits/AND_COMMUNICATION_ROUNDS.hpp"
#endif



void generateElements()
{



}




