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

#define FUNCTION searchComm__
#define RESULTTYPE DATATYPE
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

template<typename Pr, typename S>
void searchComm__ (Pr P,/*outputs*/ DATATYPE *found)
{

// allocate memory for shares
S (*dataset)[BITLENGTH] = (S ((*)[BITLENGTH])) P.alloc_Share(((int) NUM_INPUTS)*BITLENGTH);
S* element = P.alloc_Share(BITLENGTH);


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

P.prepare_receive_from((S*) dataset,0,(NUM_INPUTS)*BITLENGTH);
P.prepare_receive_from(element,1,BITLENGTH);


P.communicate();

// change to receive from


P.complete_receive_from((S*) dataset,0,(NUM_INPUTS)*BITLENGTH);
P.complete_receive_from(element,1,BITLENGTH);
/* P.receive_from_SRNG((Share*) dataset,0,BITLENGTH*n); */
/* P.receive_from_SRNG(element,1,BITLENGTH); */


for (int i = 0; i < NUM_INPUTS; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = P.Not(P.Xor(dataset[i][j], element[j]));
    }
  }
  
int c = 1;
  for (int k = BITLENGTH >> 1; k > 0; k = k >> 1) {
std::chrono::high_resolution_clock::time_point p =
            std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < NUM_INPUTS; s++) {
          P.prepare_and(dataset[s][j],dataset[s][j +1]);
      }
    }

    P.communicate(); 

    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < NUM_INPUTS; s++) {
            dataset[s][i] = P.complete_and(dataset[s][j], dataset[s][j+1]);
      }
      


    }

    double time_and = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - p).count();
    /* printf("Time measured to perform AND in batch %i: %fs \n", c, time_and / 1000000); */
      /* c++; */

  }
 
  *found = SET_ALL_ZERO(); 
  /* S sfound = P.public_val(found); */

  /* for (int i = 0; i < n; i++) { */
  /*   sfound = P.Xor(dataset[i][0],sfound); */ 

  /* } */
  
  S sfound = dataset[0][0];

  for (int i = 1; i < NUM_INPUTS; i++) {
    sfound = P.Xor(dataset[i][0],sfound); 

  }

P.prepare_reveal_to_all(sfound);
P.communicate();
*found = P.complete_Reveal(sfound);

}
// Reveal
//

void print_result(DATATYPE* var) 
{
    uint8_t v8val[sizeof(DATATYPE)];
    std::memcpy(v8val, var, sizeof(v8val));
    for (uint i = sizeof(DATATYPE); i > 0; i--)
        std::cout << std::bitset<sizeof(uint8_t)*8>(v8val[i-1]) << std::endl;
        //std::cout << v8val[i]<< std::endl;
}



