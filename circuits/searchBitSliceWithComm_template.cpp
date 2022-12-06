#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include "../networking/sockethelper.h"
#include "../networking/buffers.h"
/* Do NOT change the order of those define/include */

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
void searchComm__ (Pr P,/*outputs*/ DATATYPE &found)
{

// allocate memory for shares
S (*dataset)[BITLENGTH] = (S ((*)[BITLENGTH])) P.alloc_Share(((int) n)*BITLENGTH);
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

P.prepare_receive_from((S*) dataset,0,(n)*BITLENGTH);
P.prepare_receive_from(element,1,BITLENGTH);


P.communicate();

// change to receive from


P.complete_receive_from((S*) dataset,0,(n)*BITLENGTH);
P.complete_receive_from(element,1,BITLENGTH);
/* P.receive_from_SRNG((Share*) dataset,0,BITLENGTH*n); */
/* P.receive_from_SRNG(element,1,BITLENGTH); */


for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = P.Not(P.Xor(dataset[i][j], element[j]));
    }
  }
  

  for (int k = BITLENGTH >> 1; k > 0; k = k >> 1) {
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
          P.prepare_and(dataset[s][j],dataset[s][j +1]);
      }
    }

    P.communicate(); 

    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
            dataset[s][i] = P.complete_and(dataset[s][j], dataset[s][j+1]);
      }
      


    }

  }
 
  found = SET_ALL_ZERO(); 
  /* S sfound = P.public_val(found); */

  /* for (int i = 0; i < n; i++) { */
  /*   sfound = P.Xor(dataset[i][0],sfound); */ 

  /* } */
  
  S sfound = dataset[0][0];

  for (int i = 1; i < n; i++) {
    sfound = P.Xor(dataset[i][0],sfound); 

  }

P.prepare_reveal_to_all(sfound);
P.communicate();
found = P.complete_Reveal(sfound);

}
// Reveal
