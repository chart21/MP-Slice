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

/* #include "../protocols/dummy_Protocol.hpp" */
#include "../protocols/sharemind.hpp"
/* auxiliary functions */
/* main function */

void send()
{

      pthread_mutex_lock(&mtx_send_next); 
     sending_rounds +=1;
      pthread_cond_broadcast(&cond_send_next); //signal all threads that sending buffer contains next data
      pthread_mutex_unlock(&mtx_send_next); 
sb = 0;      
    for(int t = 0; t < num_players-1; t++)
        sending_args[t].sent_elements[sending_rounds] = NEW(DATATYPE[sending_args[t].elements_to_send[sending_rounds]]); // Allocate memory for all sending buffers for next round
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
void receive_from(DATATYPE a[], int id, int l)
{
if(id == player_id)
{
/* result = receiving_args[num_players-1].received_elements[rounds - 1][share_buffer[id]]; */
for (int i = 0; i < l; i++) {
  a[i] = player_input[share_buffer[id]];
    share_buffer[id]+=1;
}
}
else{
int offset = {id > player_id ? 1 : 0};
for (int i = 0; i < l; i++) {
a[i] = receiving_args[id - offset].received_elements[rounds-1][share_buffer[id]];
    share_buffer[id]+=1;
}
}
}

DATATYPE receive_from(int id)
{
DATATYPE result;
if(id == player_id)
{
/* result = receiving_args[num_players-1].received_elements[rounds - 1][share_buffer[id]]; */
result = player_input[share_buffer[id]];
}
else{
int offset = {id > player_id ? 1 : 0};
result = receiving_args[id - offset].received_elements[rounds-1][share_buffer[id]];
}
share_buffer[id]+=1;
return result;
}

void searchComm__ (DATATYPE dataset[n][BITLENGTH],DATATYPE element[BITLENGTH], /*outputs*/ DATATYPE found[BITLENGTH])
{
  //;
//create own shares

if(player_id == 0)
{
for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = P_share(dataset[i][j]);
  }
}
/* P_share( (DATATYPE*) dataset,n*BITLENGTH); */
}
else if(player_id == 1)
{

    /* for (int j = 0; j < BITLENGTH; j++) */ 
    /*   element[j] = P_share(element[j]); */
P_share(element,BITLENGTH);
}



send_and_receive();

// change to receive from


/* for (int i = 0; i < n; i++) { */
/*   for (int j = 0; j < BITLENGTH; j++) { */
/* dataset[i][j] = receive_from(0); */
/*   } */
/* } */
/*   for (int j = 0; j < BITLENGTH; j++) { */
/* element[j] = receive_from(1); */
/*   } */
receive_from((DATATYPE*) dataset,0,BITLENGTH*n);
receive_from(element,1,BITLENGTH);

// Players received all elements

  // Instructions (body)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < BITLENGTH; j++) {
      dataset[i][j] = NOT(P_xor(dataset[i][j],element[j]));
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


  for (int k = BITLENGTH >> 1; k > 0; k = k >> 1) {
//    DATATYPE* send_buffer = NEW(DATATYPE[k*n]);
 //   DATATYPE* receive_buffer = NEW(DATATYPE[k*n]);
   
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
        P_prepare_and(dataset[s][j],dataset[s][j +1]);
        /* dataset[s][i] = P_prepare_and_old(dataset[s][j],dataset[s][j +1]); */
      }
    }

    send_and_receive(); 
    
    
    //send_AND(buffer,b)
    // receive_AND(buffer,b)
    for (int i = 0; i < k; i++) {
        int j = i * 2;
      for (int s = 0; s < n; s++) {
        //dataset[s][i] = NOT(dataset[s][i]);
        /* for(int t = 0; t < num_players-1; t++) // for other protocols, sending buffers may be different for each player */
          /* dataset[s][i] = P_complete_and(dataset[s][i]); */
            dataset[s][i] = P_complete_and(dataset[s][j], dataset[s][j+1]);
      }
      
    /// DELETE received data to free memory! (should maybe happen in next round, or when receive buffer is at length) 


    }

    //delete[] (send_buffer, sizeof(DATATYPE));
    //delete[] (receive_buffer, sizeof(DATATYPE));
  }
 
  *found = SET_ALL_ZERO(); 
  *found = NOT(*found); //public value, therefore needs to be notted for dummy Protocol;
  for (int i = 0; i < n; i++) {
    *found = P_xor(*found,dataset[i][0]); 
    /* std::cout << dataset[i][0]; */

  }

  /* for (int i = 0; i < BITLENGTH; i++) */ 
  /*   std::cout << element[i]; */

/* std::cout << "\n"; */
P_prepare_reveal_to_all(*found);
send_and_receive();
*found = P_complete_Reveal(*found);

}
// Reveal
