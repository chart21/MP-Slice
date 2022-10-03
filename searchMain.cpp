#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <random>
#include <bitset>
#include <new>
#include <memory>
#include "arch/DATATYPE.h"
#include "circuits/searchBitSlice.c"
#include "circuits/searchBitSliceWithComm.cpp"
#include "circuits/xorshift.c"
#include "utils/timing.hpp"
#include "networking/client.c"
#include "networking/server.c"
#include "networking/sockethelper.h"

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

void print_num(DATATYPE var) 
{
    uint8_t v8val[sizeof(DATATYPE)];
    std::memcpy(v8val, &var, sizeof(v8val));
    for (uint i = sizeof(DATATYPE); i > 0; i--)
        std::cout << std::bitset<sizeof(uint8_t)*8>(v8val[i-1]) << std::endl;
        //std::cout << v8val[i]<< std::endl;
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


int main(int argc, char *argv[])
{

int player_id = atoi(argv[1]);

/// Connecting to other Players
int INPUTSLENGTH[] = {4,1};
char* ips[input_players-1];

//char* hostnames[input_players-1];
for(int i=0; i < input_players -1; i++)
{
    if(i < (argc - 1))
        ips[i] = argv[i+2];
    else
        ips[i] = (char*) "127.0.0.1";
}
pthread_mutex_init(&mtx_connection_established, NULL);
pthread_mutex_init(&mtx_start_communicating, NULL);
pthread_cond_init(&cond_successful_connection, NULL);
//DATATYPE** inputs = new DATATYPE*[input_players]; //create n pointers, each to hold a player's input
//int inputLength[] = INPUTSLENGTH;
/* for (int i = 0; i < input_players; i++) { */
/*    inputs[i] = new DATATYPE[inputLength[i]]; // initialize arrays of each player with a size */ 
/* } */


/* std::vector<DATATYPE> inputs[input_players]; */    
pthread_t receiving_threads[input_players-1];
thargs_t thrgs[input_players];
int ret;
/* printf("creating receiving servers\n"); */
for(int t=0;t<(input_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not receive from itself
    thrgs[t].num_players = input_players;
    thrgs[t].inputs = NULL;
    thrgs[t].inputs_size = 0;
    thrgs[t].threadID = t;
    thrgs[t].ip = ips[t];
    thrgs[t].hostname = (char*)"hostname";
        thrgs[t].port = (int) base_port + player_id * (input_players-1) + t; //e.g. P0 receives on base port from P1, P2 on base port + num_players from P0 6000,6002
    /* std::cout << "In main: creating thread " << t << "\n"; */
    ret = pthread_create(&receiving_threads[t], NULL, receiver, &thrgs[t]);
    if (ret){
        printf("ERROR; return code from pthread_create() is %d\n", ret);
        exit(-1);
        }
}

/// Creating sending threads

pthread_t sending_Threads[input_players];
thargs_p s_thrgs[input_players-1];
/* printf("creating receiving servers\n"); */
for(int t=0;t<(input_players-1);t++) {
    int offset = 0;
    if(t >= player_id)
        offset = 1; // player should not send to itself
     s_thrgs[t].inputs = NULL;
    s_thrgs[t].inputs_size = 0;
    s_thrgs[t].port = (int) base_port + (t+offset) * (input_players -1) + player_id - 1 + offset; //e.g. P0 sends on base port + num_players  for P1, P2 on base port + num_players for P0 (6001,6000)
    /* std::cout << "In main: creating thread " << t << "\n"; */
    ret = pthread_create(&receiving_threads[t], NULL, sender, &s_thrgs[t]);
    if (ret){
        printf("ERROR; return code from pthread_create() is %d\n", ret);
        exit(-1);
        }
}



// waiting until all threads connected

/* printf("m: locking conn \n"); */
pthread_mutex_lock(&mtx_connection_established);
/* printf("m: locked conn \n"); */
while (num_successful_connections < (input_players -1)) {
/* printf("m: unlocking conn and waiting \n"); */
pthread_cond_wait(&cond_successful_connection, &mtx_connection_established);
}
/* printf("m: done waiting, modifying conn \n"); */
num_successful_connections = 0; 
pthread_cond_broadcast(&cond_successful_connection); //signal all threads to start receiving
printf("All clients connected sucessfully, starting protocol and timer! \n");
pthread_mutex_unlock(&mtx_connection_established);
/* printf("m: unlocked conn \n"); */

clock_t time_application_start = clock ();



/// Processing Inputs ///

uint64_t (*origData)[BITS_PER_REG] = NEW(uint64_t[n][BITS_PER_REG]);
uint64_t *origElements = NEW(uint64_t[BITS_PER_REG]);

// read inputs ///


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



DATATYPE* found = NEW(DATATYPE);
funcTime("evaluating", searchComm__,dataset, elements, found);

print_num(*found);
}


