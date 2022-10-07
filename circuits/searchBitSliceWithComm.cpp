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

#include "../protocols/dummy_Protocol.hpp"

/* auxiliary functions */
/* main function */

void send()
{

      pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      pthread_mutex_unlock(&mtx_send_next); 
sb = 0;      
}

void receive(){
        rounds+=1;  
      

        // receive_data
      //wait until all sockets have finished received their last data
      pthread_mutex_lock(&mtx_receive_next);
      while(rounds > receiving_rounds) //wait until all threads received their data
          pthread_cond_wait(&cond_receive_next, &mtx_receive_next);
      pthread_mutex_unlock(&mtx_receive_next);

rb = 0;
}

void send_and_receive()
{
    send();
    receive();
}

DATATYPE receive_from(int id)
{
DATATYPE result = receiving_threads_info[id].received_elements[rounds-1][rb];
rb+=1;
return result;
}

void searchComm__ (/*inputs*/ DATATYPE dataset[n][BITLENGTH],DATATYPE element[BITLENGTH], /*outputs*/ DATATYPE found[BITLENGTH])
{
  //;

//create own shares

if(player_id == 0)
{
auto shares = new DATATYPE[n][BITLENGTH][num_players]; 
for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = P_share(dataset[i][j], shares[i][j]);
  }
}
}
else if(player_id == 1)
{

auto shares = new DATATYPE[BITLENGTH][num_players]; 
    for (int j = 0; j < BITLENGTH; j++) {
      element[j] = P_share(element[j], shares[j]);
           
}
}


send_and_receive();

// change to receive from
if(player_id != 0)
{
int rb = 1;
for (int i = 0; i < n; i++) {
  for (int j = 0; j < BITLENGTH; j++) {
dataset[i][j] = receiving_threads_info[0].received_elements[rounds-1][rb];
rb+=1;
  }
}

if(player_id != 1)
{
int offset = 0;
if(player_id < 1)
    offset = 1;
int rb = 1;
  for (int j = 0; j < BITLENGTH; j++) {
element[j] = receiving_threads_info[1- offset].received_elements[rounds-1][rb];
rb+=1;
  }
}

// Players received all elements

  // Instructions (body)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = P_xor(dataset[i][j],element[j]);
    }
  }
  
  /* for (int i = 0; i < n; i++) { */
    /* for (int j = 0; j < BITLENGTH; j++) { */
    /*     fputs(dataset[i][j] ? "1" : "0", stdout); */
    /* } */
    /* printf("\n"); */
  /* } */

    /* for (int j = 0; j < BITLENGTH; j++) { */
    /*     fputs(element[j] ? "1" : "0", stdout); */
    /* } */
    /* printf("\n"); */


  int buffer_length = 0;
  for (int k = BITLENGTH >> 1; k > 0; k = k >> 1) {
//    DATATYPE* send_buffer = NEW(DATATYPE[k*n]);
 //   DATATYPE* receive_buffer = NEW(DATATYPE[k*n]);
   
    for(int t = 0; t < num_players-1; t++)
        sending_threads_info[t].sent_elements[sending_rounds] = NEW(DATATYPE[k*n]); // Allocate memory for all sending buffers
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
        dataset[s][i] = P_prepare_and(dataset[s][j],dataset[s][j +1]);
      }
    }

    send_and_receive(); 
    
    
    //send_AND(buffer,b)
    // receive_AND(buffer,b)
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
        //dataset[s][i] = NOT(dataset[s][i]);
        for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player
            dataset[s][i] = P_complete_and(dataset[s][i]); 
      }
      
    /// DELETE received data to free memory! 


    }

    //delete[] (send_buffer, sizeof(DATATYPE));
    //delete[] (receive_buffer, sizeof(DATATYPE));
  }
 
  *found = SET_ALL_ZERO();
  for (int i = 0; i < n; i++) {
    *found = P_xor(*found,dataset[i][0]);
  }

}

P_prepare_reveal(*found);
send_and_receive();
*found = P_complete_Reveal(*found);

// Reveal




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
 
