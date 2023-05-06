#pragma once
#include "../networking/buffers.h"
#include <cstdint>

#if MAL == 1
#ifdef __SHA__
#include "../arch/SHA_256_x86.h"
#else 
#include "../arch/SHA_256.h"
#endif
#endif


void send_live()
{
sb = 0;     
for(int t = 0; t < (num_players-1); t++)
{
send_count[t] = 0;
}
    // different in PRE
    for(int t = 0; t < (num_players-1); t++)
        if(sending_rounds < sending_args[t].send_rounds - 1) // don't allocate memory for the last+1 round
        sending_args[t].sent_elements[sending_rounds + 1] = NEW(DATATYPE[sending_args[t].elements_to_send[sending_rounds + 1]]); // Allocate memory for all sending buffers for next round
    pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      /* printf("boradcasted round %i \n", sending_rounds); */
      pthread_mutex_unlock(&mtx_send_next); 
}

void receive_live(){
for(int t = 0; t < (num_players-1); t++)
    share_buffer[t] = 0;

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
    share_buffer[num_players-1] +=1;
    return player_input[share_buffer[num_players-1] -1];
}

#if MAL == 1
void store_compare_view(int player_id, DATATYPE elements_to_compare)
{
if(verify_buffer_index[player_id] == VERIFY_BUFFER)
{
    // calculate hash
    #ifdef __SHA__
    sha256_process_x86(hash_val[player_id], (uint8_t*) verify_buffer[player_id],sizeof(DATATYPE)*VERIFY_BUFFER);
    #else
    sha256_process(hash_val[player_id], (uint8_t*) verify_buffer[player_id],sizeof(DATATYPE)*VERIFY_BUFFER);
    #endif

    verify_buffer_index[player_id] = 0;
}
verify_buffer[player_id][verify_buffer_index[player_id]] = elements_to_compare;
verify_buffer_index[player_id] +=1;
}

void compare_view(int player_id)
{

    if(elements_to_compare[player_id] > 0)
    {
        //exchange 1 sha256 hash. Do to DATATYPE constraints it may need to be split up to multiple chunks
        int index_slice = 0;
        #if DATTYPE >= 256
        int hash_chunks_to_send = 1;
        #else
        int hash_chunks_to_send = 32/sizeof(DATATYPE);
        #endif
        for(int i = 0; i < hash_chunks_to_send; i++)
        {
        #if DATTYPE < 64
        send_to_live(player_id, *((DATATYPE*) ((uint8_t*) hash_val[player_id])+index_slice));
        index_slice += sizeof(DATATYPE); //hash is stored in 4 byte chunks -> need smaller slices for small DATATYPE
        #else
        send_to_live(player_id, *((DATATYPE*) hash_val[player_id]+index_slice)); //check: could this lead to memory problem with AVX512? -> align 
        index_slice += sizeof(DATATYPE)/4; //hash is stored in 4 byte chunks -> move up index by multiplier
        #endif


        receive_from_live(player_id);
        }
    }
}

void compare_views()
{
    for(int i = 0; i < num_players-1; i++)
        compare_view(i);
    communicate_();

}

#endif

