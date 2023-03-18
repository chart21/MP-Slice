#pragma once
#include "../networking/buffers.h"

void send_live()
{
sb = 0;     
send_count[0] = 0;
send_count[1] = 0;
    // different in PRE
    for(int t = 0; t < num_players-1; t++)
        sending_args[t].sent_elements[sending_rounds + 1] = NEW(DATATYPE[sending_args[t].elements_to_send[sending_rounds + 1]]); // Allocate memory for all sending buffers for next round
    pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      /* printf("boradcasted round %i \n", sending_rounds); */
      pthread_mutex_unlock(&mtx_send_next); 
}

void receive_live(){
share_buffer[0] = 0;
share_buffer[1] = 0;
rounds+=1;  
        // receive_data
      //wait until all sockets have finished received their last data
      pthread_mutex_lock(&mtx_receive_next);
      
/* std::chrono::high_resolution_clock::time_point c1 = */
/*         std::chrono::high_resolution_clock::now(); */
      while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      
/* double time = std::chrono::duration_cast<std::chrono::microseconds>( */
/*                      std::chrono::high_resolution_clock::now() - c1) */
/*                      .count(); */
      /* printf("finished waiting for receive in round %i \n", rounds - 1); */
      pthread_mutex_unlock(&mtx_receive_next);
/* printf("Time spent waiting for data chrono: %fs \n", time / 1000000); */
rb = 0;
}

void communicate_live()
{
/* #if PRE == 0 */
    send_live();
    receive_live();
/* #endif */
}

void send_to_live(int player_id, DATATYPE a)
{
/* sending_args[player_id].sent_elements[sending_args[player_id].send_rounds][send_count[player_id]] = a; */ 
#if SEND_BUFFER > 0
if(send_count[player_id] == SEND_BUFFER)
{
    send_live();
}
#endif
sending_args[player_id].sent_elements[sending_rounds][send_count[player_id]] = a; 
send_count[player_id]+=1;
}

DATATYPE receive_from_live(int player_id)
{
#if RECV_BUFFER > 0
if(share_buffer[player_id] == RECV_BUFFER)
{
    receive_live();
}
#endif
share_buffer[player_id] +=1;
return receiving_args[player_id].received_elements[rounds-1][share_buffer[player_id] - 1];
/* return receiving_args[player_id].received_elements[receiving_args[player_id].rec_rounds-1][share_buffer[player_id] -1]; */
}

#if PRE == 1
void pre_send_to_live(int player_id, DATATYPE a)
{
sending_args_pre[player_id].sent_elements[0][send_count_pre[player_id]] = a;
/* sending_args_pre[player_id].sent_elements[sending_args_pre[player_id].send_rounds][send_count[player_id]] = a; */
send_count_pre[player_id]+=1;
}

DATATYPE pre_receive_from_live(int player_id)
{
share_buffer_pre[player_id] +=1;
return receiving_args_pre[player_id].received_elements[0][share_buffer_pre[player_id] - 1];
/* return receiving_args_pre[player_id].received_elements[receiving_args_pre[player_id].rec_rounds][share_buffer[player_id] -1]; */
}
#endif

DATATYPE get_input_live()
{
    share_buffer[2] +=1;
    return player_input[share_buffer[2] -1];
}
