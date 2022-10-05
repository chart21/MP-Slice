#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include "../networking/sockethelper.h"
/* Do NOT change the order of those define/include */

/* #ifndef BITS_PER_REG */
/* #define BITS_PER_REG 64 */
/* #endif */
/* /1* including the architecture specific .h *1/ */
#include "../arch/DATATYPE.h"

#include "../protocols/dummy_Protocol.hpp"

/* auxiliary functions */
/* main function */
void searchComm__ (/*inputs*/ DATATYPE dataset__[n][BITLENGTH],DATATYPE element__[BITLENGTH], /*outputs*/ DATATYPE found__[BITLENGTH], thargs_t receiving_threads_info[], thargs_p sending_threads_info[]) {
int rounds = 0;
  // Variables declaration
  //;

  // Instructions (body)
  for (int i__ = 0; i__ < n; i__++) {
    for (int j__ = 0; j__ < BITLENGTH; j__++) {
      dataset__[i__][j__] = NOT(XOR(dataset__[i__][j__],element__[j__]));
    }
  }
  
  /* for (int i__ = 0; i__ < n; i__++) { */
    /* for (int j__ = 0; j__ < BITLENGTH; j__++) { */
    /*     fputs(dataset__[i__][j__] ? "1" : "0", stdout); */
    /* } */
    /* printf("\n"); */
  /* } */

    /* for (int j__ = 0; j__ < BITLENGTH; j__++) { */
    /*     fputs(element__[j__] ? "1" : "0", stdout); */
    /* } */
    /* printf("\n"); */


  int buffer_length = 0;
  for (int k__ = BITLENGTH >> 1; k__ > 0; k__ = k__ >> 1) {
    int k = k__;
//    DATATYPE* send_buffer = NEW(DATATYPE[k*n]);
 //   DATATYPE* receive_buffer = NEW(DATATYPE[k*n]);
   
    for(int t = 0; t < num_players-1; t++)
        sending_threads_info[t].sent_elements[sending_rounds] = NEW(DATATYPE[k*n]); // Allocate memory for all sending buffers
    int sb = 0;
    int rb = 0;
    for (int i__ = 0; i__ < k__; i__++) {
        int j__ = i__ * 2;
      for (int s__ = 0; s__ < n; s__++) {
        dataset__[s__][i__] = NOT(AND(dataset__[s__][j__],dataset__[s__][j__ +1]));
        for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
            sending_threads_info[t].sent_elements[sending_rounds][sb] = dataset__[s__][i__];
        sb += 1;
      }
    }

      
    
      //send
      pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      pthread_mutex_unlock(&mtx_send_next); 
      
        
      //receive
        rounds+=1;  
      

        // receive_data
      //wait until all sockets have finished received their last data
      pthread_mutex_lock(&mtx_receive_next);
      while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      pthread_mutex_unlock(&mtx_receive_next);


      //reset
    
    //send_AND(buffer,b)
    // receive_AND(buffer,b)
    for (int i__ = 0; i__ < k__; i__++) {
        int j__ = i__ * 2;
      for (int s__ = 0; s__ < n; s__++) {
        //dataset__[s__][i__] = NOT(dataset__[s__][i__]);
        for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
            dataset__[s__][i__] = NOT(AND(dataset__[s__][i__],receiving_threads_info[t].received_elements[rounds-1][rb]));
        rb += 1;
      }
      
    /// DELETE received data to free memory! 


    }

    //delete[] (send_buffer, sizeof(DATATYPE));
    //delete[] (receive_buffer, sizeof(DATATYPE));
  }
 
  *found__ = SET_ALL_ZERO();
  for (int i__ = 0; i__ < n; i__++) {
    *found__ = XOR(*found__,dataset__[i__][0]);
  }

}

/* Additional functions */


/* **************************************************************** */
/*                            Usuba source                          */
/*                                                                  */
/*


node search(dataset :  b4[4],element :  b4) returns found :  b1
  let
    forall i in [0,3] {
      forall j in [0,3] {
        (dataset[i][j]) = (dataset[i][j] ^ element[j])
      }
    };
    forall k in [1,0] {
      forall i in [0,1] {
        forall s in [0,3] {
          (dataset[s][i]) = (dataset[s][(i * 2)] & dataset[s][(i * 2)])
        }
      }
    };
    (found) = 0x0:b<B>1;
    forall i in [0,3] {
      (found) = (found ^ dataset[i][0])
    }
  tel


*/
 
