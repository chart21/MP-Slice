#pragma once
#define PROTOCOL 4

//0: search 1: XORNOTAND
#define FUNCTION_IDENTIFIER 0

#define DATTYPE 64
#define COMPRESS 0
#define OPT_SHARE 1

#define NO_INI 0
#define INIT 1
#define LIVE 1
#define PRE 0
#define SHARE_PREP 1
#define PARTY 2

#define INPUT 'r'
#define NUM_INPUTS 1 << 12
#define BITLENGTH 64
#define num_players 3

#define BASE_PORT 29000
int base_port = BASE_PORT; // temporary solution

#define USE_SSL 1

#define PROCESS_NUM 1

//TODO: Always use Xorshift for non-cryptographic randomness
#define RANDOM_ALGORITHM 2 // 0 = xorshift, 1 = AES_BS, 2 = AES_NI

#define CONNECTION_TIMEOUT 30 // Timeout in seconds when connecting to a socket
