#pragma once
#define PROTOCOL 4
#define FUNCTION searchComm__

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
#define NUM_INPUTS 1 << 16
#define BITLENGTH 64
#define num_players 3

#define BASE_PORT 10000
int base_port = BASE_PORT; // temporary solution
#define USE_SSL 0

#define PROCESS_NUM 1
