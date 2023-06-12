#pragma once
#include <chrono>
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
#if USE_SSL_AES == 1
uint64_t counter[num_players*multiplier][2] = {0};
#else
DATATYPE counter[num_players*multiplier][BUFFER_SIZE] = {0};
#endif
#endif
#endif
#if MAL == 1
DATATYPE* verify_buffer[num_players*multiplier]; // Verify buffer for each player
uint64_t verify_buffer_index[num_players*multiplier] = {0};

alignas(sizeof(DATATYPE)) uint32_t hash_val[num_players*multiplier][8]; // Hash value for each player
uint64_t elements_to_compare[num_players*multiplier] = {0};
#endif
//DATATYPE srng[num_players -1] = {0};
//DATATYPE* input_seed;
int num_generated[num_players*multiplier] = {0};
int pnext;
int pprev;
int pmiddle;

int use_srng_for_inputs = 1;

//int seeds[] = {10000,20000,12000}; // seeds shared between 1-0,2-0,1-2
/* std::chrono::high_resolution_clock::time_point init_start; */
/* std::chrono::high_resolution_clock::time_point init_end; */
/* std::chrono::high_resolution_clock::time_point pre_start; */
/* std::chrono::high_resolution_clock::time_point pre_end; */
/* std::chrono::high_resolution_clock::time_point live_start; */
/* std::chrono::high_resolution_clock::time_point live_end; */
