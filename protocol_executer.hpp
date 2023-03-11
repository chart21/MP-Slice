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
/* #include "circuits/searchBitSlice.c" */

#if FUNCTION_IDENTIFIER == 0
#include "searchMain_template.hpp"
#elif FUNCTION_IDENTIFIER == 1
#include "XORNOTAND_Main.hpp"
#endif

#include "circuits/xorshift.h"

#include "config.h"
#include "protocols/Protocols.h"

#include "utils/randomizer.h"
#include "utils/timing.hpp"
#include "networking/client_n.hpp"
#include "networking/server_n.hpp"

#include "networking/sockethelper.h"
#include "networking/buffers.h"

#include "protocols/CircuitInfo.hpp"


struct timespec t1, t2, p1, p2;

int modulo(int x,int N){
    return (x % N + N) %N;
}



void init_srng(uint64_t link_id, uint64_t link_seed)
{
    #if RANDOM_ALGORITHM == 0
    uint64_t gen_seeds[DATTYPE];
   for (int i = 0; i < DATTYPE; i++) {
      gen_seeds[i] = link_seed * (i+1); // replace with independant seeds in the future
   }
/* int incr = (DATTYPE -1) / 64 + 1; */
/* for (int i = 0; i < 64; i+=incr) { */
/* orthogonalize(gen_seeds+i, srng[link_id]+i); */
orthogonalize(gen_seeds, srng[link_id]);

#elif RANDOM_ALGORITHM == 1
    uint64_t gen_keys[11][DATTYPE*2];
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < DATTYPE*2; j++) {
            gen_keys[i][j] = link_seed * ((i+1)*j); // replace with independant seeds in the future
        }
    }
    for (int i = 0; i < 11; i++) {
            orthogonalize(gen_keys[i], key[link_id][i]);
            orthogonalize(gen_keys[i]+64, key[link_id][i]+1);
            }
    
#elif RANDOM_ALGORITHM == 2
#if DATTYPE >= 128
    int incr = (DATTYPE -1) / 64 + 1;
#else 
int incr = (sizeof(COUNT_TYPE) - 1) /64 +1;
#endif
    uint64_t gen_keys[11][incr];
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < incr; j++) {
            gen_keys[i][j] = link_seed * ((i+1)*j); // replace with independant seeds in the future
        }
    }
    for (int i = 0; i < 11; i++) {
    memcpy(&key[link_id][i], gen_keys[i], incr*sizeof(uint64_t));
    }

    /* for (int j = 0; j < DATTYPE*2; j++) { */
        /*     gen_keys[j] = link_seed * (j+1); // replace with independant seeds in the future */
        
    /* } */
        /*     orthogonalize(gen_keys, key[link_id]); */
        /*     orthogonalize(gen_keys+64, key[link_id]+1); */


#endif

}

void init_muetexes()
{
pthread_mutex_init(&mtx_connection_established, NULL);
pthread_mutex_init(&mtx_start_communicating, NULL);
pthread_mutex_init(&mtx_send_next, NULL);
pthread_cond_init(&cond_successful_connection, NULL);
pthread_cond_init(&cond_start_signal, NULL);
pthread_cond_init(&cond_send_next, NULL);

}

void init_circuit(std::string ips[])
{

//TODO replace with chrono
clock_t time_init_start = clock ();
std::cout << "Initialzing circuit ..." << "\n";

/* //replace with vector soon !! */
sockets_received = new int[10]{0}; 
for(int t=0;t<(num_players-1);t++) { // ???
    #if LIVE == 1 
    receiving_args[t].elements_to_rec = new int[10]{0}; //TODO: replace with vector
    sending_args[t].elements_to_send = new int[10]{0};
    #endif
    #if PRE == 1
    sending_args_pre[t].elements_to_send = new int[1]{0};
    receiving_args_pre[t].elements_to_rec = new int[1]{0};
    receiving_args_pre[t].rec_rounds = 1;
    sending_args_pre[t].send_rounds = 1;
    #endif
    }
    #if INIT == 1 && NO_INI == 0
    auto p_init = PROTOCOL_INIT(OPT_SHARE);
    auto garbage = new RESULTTYPE;
    FUNCTION<PROTOCOL_INIT,INIT_SHARE>(p_init,garbage);
    
    #if PRE == 1
    p_init.finalize(ips,receiving_args_pre,sending_args_pre);
    #else
    p_init.finalize(ips); //TODO change to new version
    #endif
    #if LIVE == 0
        export_Details_to_file();
    #endif
#endif
#if LIVE == 1 && INIT == 0 && NO_INI == 0
    init_from_file();
    finalize(ips);
#endif

clock_t time_init_finished = clock ();
/* printf("creating receiving servers\n"); */
printf("Time measured to initialize program: %fs \n", double((time_init_finished - time_init_start)) / CLOCKS_PER_SEC);

}


#if PRE == 1
void preprocess_circuit(std::string ips[])
{
pthread_t sending_Threads_pre[num_players-1];
pthread_t receiving_threads_pre[num_players-1];
int ret_pre;

    for(int t=0;t<(num_players-1);t++) {
        ret_pre = pthread_create(&receiving_threads_pre[t], NULL, receiver, &receiving_args_pre[t]);
        if (ret_pre){
            printf("ERROR; return code from pthread_create() is %d\n", ret_pre);
            exit(-1);
            }
    }

    /// Creating sending threads
    for(int t=0;t<(num_players-1);t++) {
        ret_pre = pthread_create(&sending_Threads_pre[t], NULL, sender, &sending_args_pre[t]);
        if (ret_pre){
            printf("ERROR; return code from pthread_create() is %d\n", ret_pre);
            exit(-1);
            }
    }



    // waiting until all threads connected

    pthread_mutex_lock(&mtx_connection_established);
    while (num_successful_connections < 2 * (num_players -1)) {
    pthread_cond_wait(&cond_successful_connection, &mtx_connection_established);
    }
    num_successful_connections = -1; 
    pthread_cond_broadcast(&cond_start_signal); //signal all threads to start receiving
    printf("All parties connected sucessfully, starting protocol and timer! \n");
    pthread_mutex_unlock(&mtx_connection_established);



std::cout << "Preprocessing phase ..." << "\n";
clock_t time_pre_function_start = clock ();
clock_gettime(CLOCK_REALTIME, &p1);
std::chrono::high_resolution_clock::time_point p =
            std::chrono::high_resolution_clock::now();

#if PROTOCOL_PRE == -1
            // receive only
    #else
        auto p_pre = PROTOCOL_PRE(OPT_SHARE);
        auto garbage_PRE = new RESULTTYPE;
        FUNCTION<PROTOCOL_PRE,INIT_SHARE>(p_pre,garbage_PRE);
    #endif
    // manual send

    sb = 0;      
    pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      pthread_mutex_unlock(&mtx_send_next); 

    
    // manual receive

    rounds+=1;  
        // receive_data
      //wait until all sockets have finished received their last data
    pthread_mutex_lock(&mtx_receive_next);
      
    while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      
    pthread_mutex_unlock(&mtx_receive_next);

    rb = 0;
    

    double time_pre = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - p)
                         .count();
    /* searchComm__<Sharemind,DATATYPE>(protocol,*found); */
    clock_gettime(CLOCK_REALTIME, &p2);
    double accum_pre = ( p2.tv_sec - p1.tv_sec )
    + (double)( p2.tv_nsec - p1.tv_nsec ) / (double) 1000000000L;
    clock_t time_pre_function_finished = clock ();
    /* printf("Time measured to read and receive inputs: %fs \n", double((time_data_received - time_application_start)) / CLOCKS_PER_SEC); */
   

    // Join threads to avoid address rebind
    for(int t=0;t<(num_players-1);t++) {
    pthread_join(receiving_threads_pre[t],NULL);
    pthread_join(sending_Threads_pre[t],NULL);
    }
    printf("Time measured to perform preprocessing clock: %fs \n", double((time_pre_function_finished - time_pre_function_start)) / CLOCKS_PER_SEC);
    printf("Time measured to perform preprocessing getTime: %fs \n", accum_pre);
    printf("Time measured to perform preprocessing chrono: %fs \n", time_pre / 1000000);

#if LIVE == 1
    // reset all variables
num_successful_connections = 0;
delete[] sockets_received;
sockets_received = new int[10]{0};
rb = 0;
sb = 0;
rounds = 0;
sending_rounds = 0;
receiving_rounds = 0;
    #if INIT == 0 && NO_INI == 0
         init_from_file();
        finalize(ips);
    #else
        auto p_init = PROTOCOL_INIT(OPT_SHARE);
        p_init.finalize(ips);
        /* p_init.finalize(ips,receiving_args,sending_args); */
    #endif
#endif

}
#endif
#if LIVE == 1
void live_circuit()
{
    pthread_t sending_Threads[num_players-1];
pthread_t receiving_threads[num_players-1];
int ret;

//TODO check, recently commented
    for(int t=0;t<(num_players-1);t++) {
        ret = pthread_create(&receiving_threads[t], NULL, receiver, &receiving_args[t]);
        if (ret){
            printf("ERROR; return code from pthread_create() is %d\n", ret);
            exit(-1);
            }
    }

    /// Creating sending threads

    /* printf("creating receiving servers\n"); */


    for(int t=0;t<(num_players-1);t++) {
        ret = pthread_create(&sending_Threads[t], NULL, sender, &sending_args[t]);
        if (ret){
            printf("ERROR; return code from pthread_create() is %d\n", ret);
            exit(-1);
            }
    }



    // waiting until all threads connected

    /* printf("m: locking conn \n"); */
    pthread_mutex_lock(&mtx_connection_established);
    /* printf("m: locked conn \n"); */
    while (num_successful_connections < 2 * (num_players -1)) {
    /* printf("m: unlocking conn and waiting \n"); */
    pthread_cond_wait(&cond_successful_connection, &mtx_connection_established);
    }
    /* printf("m: done waiting, modifying conn \n"); */
    num_successful_connections = -1; 
    pthread_cond_broadcast(&cond_start_signal); //signal all threads to start receiving
    printf("All parties connected sucessfully, starting protocol and timer! \n");
    pthread_mutex_unlock(&mtx_connection_established);
    /* printf("m: unlocked conn \n"); */




    /// Processing Inputs ///
    /* Sharemind protocol = Sharemind(); */
    clock_t time_function_start = clock ();
    clock_gettime(CLOCK_REALTIME, &t1);
    std::chrono::high_resolution_clock::time_point c1 =
            std::chrono::high_resolution_clock::now();
    
    auto p_live = PROTOCOL_LIVE(OPT_SHARE);
    auto result = new RESULTTYPE;
    FUNCTION<PROTOCOL_LIVE,SHARE>(p_live,result);
    

    double time = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - c1)
                         .count();
    /* searchComm__<Sharemind,DATATYPE>(protocol,*found); */
    clock_gettime(CLOCK_REALTIME, &t2);
    double accum = ( t2.tv_sec - t1.tv_sec )
    + (double)( t2.tv_nsec - t1.tv_nsec ) / (double) 1000000000L;
    print_result(result); //different for other functions
    clock_t time_function_finished = clock ();
    /* printf("Time measured to read and receive inputs: %fs \n", double((time_data_received - time_application_start)) / CLOCKS_PER_SEC); */
    printf("Time measured to perform computation clock: %fs \n", double((time_function_finished - time_function_start)) / CLOCKS_PER_SEC);
    printf("Time measured to perform computation getTime: %fs \n", accum);
    printf("Time measured to perform computation chrono: %fs \n", time / 1000000);
    // Join threads to ensure closing of sockets
    for(int t=0;t<(num_players-1);t++) {
        pthread_join(receiving_threads[t],NULL);
        pthread_join(sending_Threads[t],NULL);
    }

}
#endif

void executeProgram(int argc, char *argv[], int process_id, int process_num)
{
player_id = PARTY;
#if num_players == 3
pnext = (player_id == 1);
pprev = (player_id != 1);
#elif num_players == 4
pnext = (player_id + 1) % 4;
pprev = (player_id + 3) % 4;
pmiddle = (player_id + 2) % 4;
#endif

#if num_players == 3
init_srng(pprev, modulo((player_id - 1),  num_players) + 5000);
init_srng(pnext,player_id + 5000);
init_srng(num_players-1, player_id+6000); // used for sharing inputs
#elif num_players == 4
//new logic
init_srng(0, (player_id+1) * 1 + 5000); // 
init_srng(1, (player_id+1) * 2 + 5000); // 1*2
init_srng(2, (player_id+1) * 3 + 5000); // 1*3
init_srng(3, (player_id+1) * 4 + 5000); // 1*4
#endif

/// Connecting to other Players
std::string ips[num_players-1];

//char* hostnames[num_players-1];
for(int i=0; i < num_players -1; i++)
{
    if(i < argc - 1 )
        ips[i] = std::string(argv[i+1]);
    else
    {
        ips[i] = "127.0.0.1";
    }
}


init_muetexes();


generateElements();

init_circuit(ips);

#if PRE == 1
    preprocess_circuit(ips);
#endif

#if LIVE == 1
    live_circuit();
#endif

}


