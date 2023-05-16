#pragma once
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
#include <chrono>
/* Do NOT change the order of those define/include */
#include <bitset>
#include <cstring>
/* #ifndef BITS_PER_REG */
/* #define BITS_PER_REG 64 */
/* #endif */
/* /1* including the architecture specific .h *1/ */
#include "../arch/DATATYPE.h"
/* #include "../protocols/Protocol.hpp" */
/* #include "../protocols/sharemind_template.hpp" */
/* #include "../protocols/sharemind_init_template.hpp" */
/* #include "../protocols/dummy_Protocol.hpp" */
/* #include "../protocols/dummy_Protocol.hpp" */
/* #include "../protocols/sharemind.hpp" */
/* #include "../protocols/replicated_template.hpp" */
/* #include "../protocols/replicated_init_template.hpp" */
/* auxiliary functions */
/* main function */

#include "../protocols/Protocols.h"

#define FUNCTION debug
#define RESULTTYPE DATATYPE[num_players][BITLENGTH]
/* void receive_from_SRNG(DATATYPE a[], int id, int l) */
/* { */
/* if(id == player_id) */
/* { */
/* for (int i = 0; i < l; i++) { */
/*   a[i] = player_input[share_buffer[id]]; */
/*   a[i] = P_share_SRNG(a[i]); */  
/*   share_buffer[id]+=1; */
/* } */
/* } */
/* else{ */
/* int offset = {id > player_id ? 1 : 0}; */
/* for (int i = 0; i < l; i++) { */
/*     a[i] = getRandomVal(id - offset); */
/* } */
/* } */
/* } */

void compare(DATATYPE var[num_players][BITLENGTH]) 
{
auto inputs = new DATATYPE[num_players][BITLENGTH];
for(int i = 0; i < num_players; i++)
{
int num_erros = 0;
for(int j = 0; j < BITLENGTH; j++)
{
inputs[i][j] = SET_ALL_ONE();
if(j == i)
    inputs[i][j] = SET_ALL_ZERO();
if(inputs[i][j] != var[i][j])
    num_erros++;
}
    std::cout << num_erros << " Errors in compare, input from player " << i << std::endl;

}

}

template<typename Pr, typename S>
void debug (Pr P,/*outputs*/ DATATYPE result[num_players][BITLENGTH])
{

// allocate memory for shares
S (*inputs)[BITLENGTH] = (S ((*)[BITLENGTH])) P.alloc_Share(((int) num_players)*BITLENGTH);

/* if(player_id == 0) */
/* { */
/* /1* for (int i = 0; i < n; i++) { *1/ */
/* /1*     for (int j = 0; j < BITLENGTH; j++) { *1/ */
/* /1*       dataset[i][j] = P.share(dataset[i][j]); *1/ */
/* /1*   } *1/ */
/* /1* } *1/ */
/* /1* P_share( (DATATYPE*) dataset,n*BITLENGTH); *1/ */
/* P.share( (S*) dataset,(n)*BITLENGTH); */
/* } */
/* else if(player_id == 1) */
/* { */

/* P.share(element,BITLENGTH); */
/*     /1* for (int j = 0; j < BITLENGTH; j++) *1/ */ 
/*     /1*   element[j] = P.share(element[j]); *1/ */
/* /1* P_share(element,BITLENGTH); *1/ */
/* } */
P.prepare_receive_from(inputs[0] ,P0,BITLENGTH);
P.prepare_receive_from(inputs[1] ,P1,BITLENGTH);
P.prepare_receive_from(inputs[2] ,P2,BITLENGTH);
#if num_players > 3
P.prepare_receive_from(inputs[3] ,P3,BITLENGTH);
#endif

P.communicate();

// change to receive from
P.complete_receive_from(inputs[0] ,P0,BITLENGTH);
P.complete_receive_from(inputs[1] ,P1,BITLENGTH);
P.complete_receive_from(inputs[2] ,P2,BITLENGTH);
#if num_players > 3
P.complete_receive_from(inputs[3] ,P3,BITLENGTH);
#endif
P.communicate();

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    P.prepare_reveal_to_all(inputs[j][i]);
}
}
P.communicate();

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    result[j][i] = P.complete_Reveal(inputs[j][i]);
}
}
P.communicate();


std::cout << "Testing secret sharing and revealing: " << std::endl;
compare(result);



for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    P.prepare_and(inputs[j][i],inputs[j][i],inputs[j][i]);
}
}
P.communicate();
for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    P.complete_and(inputs[j][i]);
}
}

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    P.prepare_reveal_to_all(inputs[j][i]);
}
}
P.communicate();

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    result[j][i] = P.complete_Reveal(inputs[j][i]);
}
}

std::cout << "Testing and gates: " << std::endl;
compare(result);

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    inputs[j][i] = P.Xor(inputs[j][i],inputs[j][i]);
    if(i != j)
        inputs[j][i] = P.Not(inputs[j][i]);
}
}

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    P.prepare_reveal_to_all(inputs[j][i]);
}
}
P.communicate();

for(int j = 0; j < num_players; j++)
{

for (int i = 0; i < BITLENGTH; i++) {

    result[j][i] = P.complete_Reveal(inputs[j][i]);
}
}

std::cout << "Testing NOT, XOR gates: " << std::endl;
compare(result);


}
// Reveal
//

/* void print_result(DATATYPE* var) */ 
/* { */
/*     uint8_t v8val[sizeof(DATATYPE)]; */
/*     std::memcpy(v8val, var, sizeof(v8val)); */
/*     for (uint i = sizeof(DATATYPE); i > 0; i--) */
/*         std::cout << std::bitset<sizeof(uint8_t)*8>(v8val[i-1]) << std::endl; */
/*         //std::cout << v8val[i]<< std::endl; */
/* } */


void print_result(DATATYPE var[][BITLENGTH]) 
{
}
    




