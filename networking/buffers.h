#pragma once
#include "sockethelper.h"
int player_id;
sender_args sending_args[num_players];
receiver_args receiving_args[num_players];

#if PRE == 1
sender_args sending_args_pre[num_players];
receiver_args receiving_args_pre[num_players];
#endif

int rounds;
int rb;
int sb;
int send_count[num_players] = {0};
int share_buffer[num_players] = {0}; //TODO: move to protocol layer
int send_count_pre[num_players] = {0};
int share_buffer_pre[num_players] = {0}; //TODO: move to protocol layer
int reveal_buffer[num_players] = {0};
int total_comm;
int* elements_per_round;
int input_length[num_players] = {0};
int reveal_length[num_players] = {0};
DATATYPE* player_input;

#if num_players == 4
    #define multiplier 2
#else
    #define multiplier 1
#endif
#if RANDOM_ALGORITHM == 0
DATATYPE srng[num_players*multiplier][64]{0};
#elif RANDOM_ALGORITHM == 1
DATATYPE counter[num_players*multiplier][128]{0};
DATATYPE cipher[num_players*multiplier][128]{0};
DATATYPE key[num_players*multiplier][11][128]{0};
#elif RANDOM_ALGORITHM == 2
#if DATTYPE >= 128
DATATYPE counter[num_players*multiplier]{0};
DATATYPE key[num_players*multiplier][11]{0};
#else
#define BUFFER_SIZE 128/DATTYPE 
DATATYPE counter[num_players*multiplier][BUFFER_SIZE]{0};
#endif
#endif

//DATATYPE srng[num_players -1] = {0};
//DATATYPE* input_seed;
int num_generated[num_players*multiplier] = {0};
int pnext;
int pprev;
int pmiddle;

int use_srng_for_inputs = 1;

//int seeds[] = {10000,20000,12000}; // seeds shared between 1-0,2-0,1-2

