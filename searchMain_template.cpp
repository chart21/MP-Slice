#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <cstring>
#include <random>
#include <bitset>
#include <new>
#include <memory>
#include "arch/DATATYPE.h"
#include "circuits/searchBitSlice.c"
#include "circuits/searchBitSliceWithComm_template.cpp"
#include "circuits/xorshift.h"
/* #include "protocols/sharemind_init_template.hpp" */
/* #include "protocols/sharemind_template.hpp" */
#include "protocols/replicated_template.hpp"
#include "protocols/replicated_init_template.hpp"

#include "utils/timing.hpp"
#include "networking/client.c"
#include "networking/server.c"
#include "networking/sockethelper.h"
#include "networking/buffers.h"
#include "utils/printing.hpp"

struct timespec t1, t2;
int modulo(int x,int N){
    return (x % N + N) %N;
}


void print_bool(uint8_t* found)
{
     for (int j = 0; j < BITS_PER_REG; j++)
         std::cout << +found[j];
    std::cout << '\n';
}


void search_Compare(uint64_t origData[n][BITS_PER_REG], uint64_t origElements[], uint8_t* found)
{
 for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITS_PER_REG; j++) {
        if(origData[i][j] == origElements[j])
            found[j] = 1;
    }   
}
}


void insertManually(DATATYPE dataset[n][BITLENGTH], DATATYPE elements[n], uint64_t origData[n][BITS_PER_REG], uint64_t origElements[], int c, int b, uint64_t numElement, uint64_t numDataset ){

unorthogonalize(elements, origElements);

for (int i = 0; i < n; i++) {
 unorthogonalize(dataset[i], origData[i]);   
}
origData[c][b] = numDataset;
origElements[b] = numElement;
std::cout << origData[c][b] << origElements[b] << std::endl;


uint8_t* cfound = new uint8_t[BITS_PER_REG]{0};
funcTime("Plain search", search_Compare, origData, origElements, cfound);
print_bool(cfound);




orthogonalize(origElements, elements);

for (int i = 0; i < n; i++) {
 orthogonalize(origData[i], dataset[i]);   
}
}
void randomizeInputs(DATATYPE dataset[n][BITLENGTH], DATATYPE elements[n])
{
    // init randomization
uint64_t* iseed = NEW(uint64_t[BITS_PER_REG]);
for (int i = 0; i < BITS_PER_REG; i++) {
    iseed[i] = rand();
}
DATATYPE* seed = new DATATYPE[BITLENGTH];
funcTime("single ortho", orthogonalize,iseed, seed);

//generate random data
for (int i = 0; i < n; i++) {
     xor_shift__(seed, dataset[i]);
 }
 funcTime("xor_shift",xor_shift__,seed, elements);
}

//init 1 srng per link
void init_srng(uint64_t link_id, uint64_t link_seed)
{
    uint64_t gen_seeds[BITS_PER_REG];
   for (int i = 0; i < BITS_PER_REG; i++) {
      gen_seeds[i] = link_seed * (i+1); // replace with independant seeds in the future
   }

//DATATYPE (*srngl)[BITLENGTH] = NEW(DATATYPE[num_players-1][BITLENGTH]);
orthogonalize(gen_seeds, srng[link_id]);
}

void init_comm()
{


// sharing
input_length[0] = BITLENGTH * n;
input_length[1] = BITLENGTH;

// revealing
reveal_length[0] = 1;
reveal_length[1] = 1;
reveal_length[2] = 1;

total_comm = 2 - use_srng_for_inputs;

// function

for (int k = BITLENGTH >> 1; k > 0; k = k >> 1)
{
    total_comm = total_comm + 1;
}
elements_per_round = new int[total_comm];



//function

int i = 1 - use_srng_for_inputs;
for (int k = BITLENGTH >> 1; k > 0; k = k >> 1)
{
    elements_per_round[i] = k*n;
    i+=1;
}




}

int main(int argc, char *argv[])
{
//init_comm();



uint64_t (*origData)[BITS_PER_REG] = NEW(uint64_t[n][BITS_PER_REG]);
uint64_t *origElements = NEW(uint64_t[BITS_PER_REG]);

// read inputs ///


// Processing inputs



//Slicing inputs
//DATATYPE (*dataset)[BITLENGTH] = malloc(sizeof(DATATYPE[n][BITLENGTH]));
DATATYPE (*dataset)[BITLENGTH] = NEW(DATATYPE[n][BITLENGTH]);
DATATYPE* elements = NEW(DATATYPE[BITLENGTH]);
for (int i = 0; i < n; i++) {
 orthogonalize(origData[i], dataset[i]);   
}
orthogonalize(origElements, elements);

// generate random input data instead of reading from file
funcTime("generating random inputs",randomizeInputs,dataset,elements);



//modify certain data to test functionality
//
insertManually(dataset, elements, origData, origElements, 1,7 , 200, 200);




player_id = atoi(argv[1]);
pnext = (player_id == 1);
pprev = (player_id != 1);
//pnext = player_id == 0 ? 0 : player_id == 1 ? 1 : 0;
/* pprev = player_id == 0 ? 1 : player_id == 1 ? 0 : 1; */
//init_srng(0,pprev+5000);
//init_srng(1,player_id+5000);
srng = NEW(DATATYPE[num_players][BITLENGTH]);


init_srng(pprev, modulo((player_id - 1),  num_players) + 5000);
init_srng(pnext,player_id + 5000);
init_srng(num_players-1, player_id+6000); // used for sharing inputs

if(player_id == 0)
{
    player_input = (DATATYPE*) dataset;
}
else if(player_id == 1)
    player_input = elements;


DATATYPE* found = NEW(DATATYPE);


/// Connecting to other Players
char* ips[num_players-1];

//char* hostnames[num_players-1];
for(int i=0; i < num_players -1; i++)
{
    if(i < (argc - 1))
        ips[i] = argv[i+2];
    else
        ips[i] = (char*) "127.0.0.1";
}
pthread_mutex_init(&mtx_connection_established, NULL);
pthread_mutex_init(&mtx_start_communicating, NULL);
pthread_cond_init(&cond_successful_connection, NULL);
//DATATYPE** inputs = new DATATYPE*[num_players]; //create n pointers, each to hold a player's input
//int inputLength[] = INPUTSLENGTH;
/* for (int i = 0; i < num_players; i++) { */
/*    inputs[i] = new DATATYPE[inputLength[i]]; // initialize arrays of each player with a size */ 
/* } */


/* std::vector<DATATYPE> inputs[num_players]; */    
pthread_t sending_Threads[num_players-1];
pthread_t receiving_threads[num_players-1];
int ret;

sockets_received = new int[10]{0};
//replace with vector soon
for(int t=0;t<(num_players-1);t++) {
    receiving_args[t].elements_to_rec = new int[10]{0};
    sending_args[t].elements_to_send = new int[10]{0};
}
Replicated_init init_protocol = Replicated_init();
/* Sharemind_init init_protocol = Sharemind_init(); */
DATATYPE garbage = SET_ALL_ZERO();
clock_t time_init_start = clock ();
searchComm__<Replicated_init,Share>(init_protocol,garbage);
init_protocol.finalize(ips);
clock_t time_init_finished = clock ();
/* printf("creating receiving servers\n"); */
printf("Time measured to initialize program: %fs \n", double((time_init_finished - time_init_start)) / CLOCKS_PER_SEC);

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
pthread_cond_broadcast(&cond_successful_connection); //signal all threads to start receiving
printf("All parties connected sucessfully, starting protocol and timer! \n");
pthread_mutex_unlock(&mtx_connection_established);
/* printf("m: unlocked conn \n"); */




/// Processing Inputs ///
Replicated protocol = Replicated();
/* Sharemind protocol = Sharemind(); */
clock_t time_function_start = clock ();
clock_gettime(CLOCK_REALTIME, &t1);
std::chrono::high_resolution_clock::time_point c1 =
        std::chrono::high_resolution_clock::now();
searchComm__<Replicated,Share>(protocol,*found);
double time = std::chrono::duration_cast<std::chrono::microseconds>(
                     std::chrono::high_resolution_clock::now() - c1)
                     .count();
/* searchComm__<Sharemind,DATATYPE>(protocol,*found); */
clock_gettime(CLOCK_REALTIME, &t2);
double accum = ( t2.tv_sec - t1.tv_sec )
+ (double)( t2.tv_nsec - t1.tv_nsec ) / (double) 1000000000L;
print_num(*found);
clock_t time_function_finished = clock ();
/* printf("Time measured to read and receive inputs: %fs \n", double((time_data_received - time_application_start)) / CLOCKS_PER_SEC); */
printf("Time measured to perform computation clock: %fs \n", double((time_function_finished - time_function_start)) / CLOCKS_PER_SEC);
printf("Time measured to perform computation getTime: %fs \n", accum);
printf("Time measured to perform computation chrono: %fs \n", time / 1000000);
/* printf("Time measured in total: %fs \n", double((time_computation_finished - time_application_start)) / CLOCKS_PER_SEC); */
}
