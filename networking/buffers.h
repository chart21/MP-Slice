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
int reveal_buffer[num_players] = {0};
int total_comm;
int* elements_per_round;
int input_length[num_players] = {0};
int reveal_length[num_players] = {0};
DATATYPE* player_input;
DATATYPE(*srng)[BITLENGTH];

#if RANDOM_ALGORITHM == 1
DATATYPE(*counter)[128];
DATATYPE(*cipher)[128];
DATATYPE(*key)[11][128];
#elif RANDOM_ALGORITHM == 2
DATATYPE(*counter);
DATATYPE(*key)[11];
#endif

//DATATYPE srng[num_players -1] = {0};
//DATATYPE* input_seed;
int num_generated[num_players] = {0};
int pnext;
int pprev;

int use_srng_for_inputs = 1;

//int seeds[] = {10000,20000,12000}; // seeds shared between 1-0,2-0,1-2
