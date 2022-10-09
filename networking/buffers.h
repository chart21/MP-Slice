#pragma once
#include "sockethelper.h"
int player_id;
sender_args sending_args[num_players];
receiver_args receiving_args[num_players];
int rounds;
int rb;
int sb;
int share_buffer[num_players] = {0};
int reveal_buffer[num_players] = {0};
int total_comm;
int* elements_per_round;
int input_length[num_players] = {0};
int reveal_length[num_players] = {0};
DATATYPE* player_input;
DATATYPE* seed;
