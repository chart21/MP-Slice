#include <iterator>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include "sockethelper.h"

#include "../arch/DATATYPE.h"
#define MAXDATASIZE 100 // max number of bytes we can get at once -> currently not used 

 
void *receiver(void* threadParameters)
{

	int sockfd, numbytes;  
	//char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
   
    char port[4];
    sprintf(port, "%d", ((receiver_args*) threadParameters)->port);

	if ((rv = getaddrinfo(((receiver_args*) threadParameters)->ip, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and connect to the first we can
	p = NULL;
    printf("Player %i: Attempting to connect to Player %i ... \n",((receiver_args*) threadParameters)->player_id, ((receiver_args*) threadParameters)->connected_to);
    while(p == NULL){
    for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

    sleep(1);	
			
    }

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("Player %i: Connected to Player %i \n",  ((receiver_args*) threadParameters)->player_id, ((receiver_args*) threadParameters)->connected_to);

	freeaddrinfo(servinfo); // all done with this structure
   
    //signal main thread that connection was established
    /* printf("Player: Locking conn \n"); */
    pthread_mutex_lock(&mtx_connection_established);
    /* printf("Player: Locked conn \n"); */
    num_successful_connections += 1; 
    /* printf("Player: Modyfied conn \n"); */
    /* pthread_mutex_unlock(&mtx_connection_established); */
    /* printf("Player: Unlocked conn \n"); */
    /* pthread_cond_signal(&cond_successful_connection); */
    /* printf("Player: Signal conn \n"); */
    /* //get start signal from main */
    /* printf("Player: Locking conn \n"); */
    /* pthread_mutex_lock(&mtx_connection_established); */
    /* printf("Player: Locked conn \n"); */
    if(num_successful_connections == 2 * (((receiver_args*) threadParameters)->player_count -1)) {
        pthread_cond_signal(&cond_successful_connection); //signal main thread that all threads have connected
        /* printf("Player: Signal conn \n"); */
    }
    pthread_mutex_unlock(&mtx_connection_established);
    /* printf("Player: Unlocked conn \n"); */

    pthread_mutex_lock(&mtx_start_communicating); 
    while (num_successful_connections != -1) { // wait for start signal from main thread
        /* printf("Player: Unlocking conn and waiting for signal \n"); */ 
        pthread_cond_wait(&cond_successful_connection, &mtx_start_communicating);
    }
        /* printf("Player: Done waiting, unlocking \n"); */
        pthread_mutex_unlock(&mtx_start_communicating);
    
        int rounds = 0;
        /* pthread_mutex_lock(&mtx_receive_next); */
        /* while(receiving_rounds != -1) */
        while(rounds < ((receiver_args*) threadParameters)->rec_rounds)
        {
            // Allocate new memory for received data, check correctness
            ((receiver_args*) threadParameters)->received_elements[rounds] = NEW(DATATYPE[((receiver_args*) threadParameters)->elements_to_rec[rounds]]);
            /* printf("start rec \n"); */
        
if(((receiver_args*) threadParameters)->elements_to_rec[rounds] > 0) //should data be received in this round?
{
    int elements_to_rec =  ((receiver_args*) threadParameters)->elements_to_rec[rounds];
    elements_to_rec = elements_to_rec * sizeof(DATATYPE);
    printf("receiving %i bytes \n", elements_to_rec);

            if ((recv(sockfd, ((char*) ((receiver_args*) threadParameters)->received_elements[rounds]), elements_to_rec, MSG_WAITALL)) == -1) {
                perror("recv");
                exit(1);
            } 
}
//If all sockets received, signal main_thread
            pthread_mutex_lock(&mtx_data_received);
            sockets_received[rounds] += 1;
            if(sockets_received[rounds] == ((receiver_args*) threadParameters)->player_count -1) 
            {
                pthread_mutex_lock(&mtx_receive_next); // Mutex probably neccessary if one thread is alrady one round further
                receiving_rounds += 1; //increase global receiving_rounds
                pthread_cond_signal(&cond_receive_next); // signal main thread that receiving is finished
                pthread_mutex_unlock(&mtx_receive_next);
            }
                pthread_mutex_unlock(&mtx_data_received);
                rounds += 1;
            //signal main thread that data was received
            /* pthread_mutex_lock(&mtx_data_received); */
            /* num_data_received += 1; */ 
            /* pthread_mutex_unlock(&mtx_data_received); */
            /* pthread_cond_signal(&cond_data_received); */
       }
    close(sockfd);

    pthread_exit( NULL );
}

