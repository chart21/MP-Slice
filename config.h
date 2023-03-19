#pragma once
#define PROTOCOL 4
#if PROTOCOL < 7
#define num_players 3
#else
#define num_players 4
#endif

//0: search 1: XORNOTAND, 2: AND 1 comm round 3: AND 1000 comm rounds 
#define FUNCTION_IDENTIFIER 0


// Registersize to use for SIMD parallelization (Bitslicing/vectorization)
#define DATTYPE 64

// Compress binary data into chars before sending them over the netowrk? Only relevant for DATTYPE = 1
#define COMPRESS 0

// Use optimized secret sharing? Often utilizes SRNG instead of secret sharing with communication
#define OPT_SHARE 1


// Use the initialization phase or import initiliazation data from a file?
#define NO_INI 0

// Use the initialization phase or import initiliazation data from a file?
#define INIT 1

// Use the online phase?
#define LIVE 1

// Use the offline phase?
#define PRE 0

// Allow sharing of inputs in offline phase
#define SHARE_PREP 1

// Party ID (starting from 0)
#define PARTY 3

// Use random inputs or inputs from a file? TODO: File inputs to be implemented
#define INPUT 'r'

// Number of inputs (depends on the problem)
#define NUM_INPUTS 4096

// Bitlength of integers
#define BITLENGTH 64

// Number of players in the protocol

// Starting port for required port range of the sockets
#define BASE_PORT 29000
int base_port = BASE_PORT; // temporary solution

// Use SSL encrypted communication?
#define USE_SSL 1

// Number of parallel processes to use
#define PROCESS_NUM 1

// 0 = xorshift, 1 = AES_BS, 2 = AES_NI
#define RANDOM_ALGORITHM 2 

// Timeout in seconds when connecting to a socket
#define CONNECTION_TIMEOUT 30 

// How many gates should be buffered until sending them to the receiving party? 0 means the data of an entire communication round is buffered
#define SEND_BUFFER 0

// How many reciving messages should be buffered until the main thread is signaled that data is ready? 0 means that all data of a communication round needs to be ready before the main thread is signaled.
#define RECV_BUFFER 0

// Print additional info?
#define PRINT 0
