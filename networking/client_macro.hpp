#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#if USE_SSL == 1
#include <openssl/ssl.h>
#include <openssl/err.h>
#define SOCKET_TYPE SSL*
//change to read all
#else
#define SOCKET_TYPE int
#endif
#ifndef BOOL_COMPRESS
#define DTYPE DATATYPE
#else
#define DTYPE uint8_t
#endif

#include "sockethelper.h"

int receive_all(SOCKET_TYPE s, DTYPE *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len * sizeof(DATATYPE); // how many we have left to receive
    int n_rec;

    while(total < *len) {
        #if USE_SSL == 1
        n_rec = SSL_read(s, ((char*) buf)+total, bytesleft);
        #else
        n_rec = recv(s, ((char*) buf)+total, bytesleft, 0);
        #endif
        if (n_rec == -1) { break; }
        total += n_rec;
        bytesleft -= n_rec;
    }

    *len = total; // return number actually received here

    return n_rec==-1?-1:0; // return -1 on failure, 0 on success
}

void *receiver(void* threadParameters)
{
    // Create a socket
    int sockfd;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }

    // Resolve the host and port
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* results;
    
    char port[4];
    sprintf(port, "%d", ((receiver_args*) threadParameters)->port);

    int status = getaddrinfo(((receiver_args*) threadParameters)->ip, port, &hints, &results);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << "\n";
        exit(1);
    }

    // Connect to the server
    int connected = 0;
    while (!connected) {
        for (addrinfo* rp = results; rp != NULL; rp = rp->ai_next) {
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
                connected = 1;
                break;
            }
        }
        /* if (!connected) { */
        /*     std::cerr << "Failed to connect to server, retrying...\n"; */
        /*     sleep(1); */
        /* } */
    }
    freeaddrinfo(results);

    #if USE_SSL == 1
        SSL_library_init();
        SSL_load_error_strings();

        SSL_CTX* ctx = NULL;
        SSL* ssl = NULL;
        ctx = SSL_CTX_new(SSLv23_client_method());
      if (ctx == nullptr) {
        throw std::runtime_error("Error creating SSL context");
      }

      // Create an SSL structure
      ssl = SSL_new(ctx);
      if (ssl == nullptr) {
        throw std::runtime_error("Error creating SSL structure");
      }

      // Connect the SSL structure to the socket
      if (SSL_set_fd(ssl, sockfd) != 1) {
        throw std::runtime_error("Error connecting SSL structure to socket");
      }

      // Perform the SSL handshake
      if (SSL_connect(ssl) != 1) {
        throw std::runtime_error("Error performing SSL handshake");
      }
    /* SSL_connect(ssl); */
    #endif
	printf("Player %i: Client Connected to Player %i \n",  ((receiver_args*) threadParameters)->player_id, ((receiver_args*) threadParameters)->connected_to);
   
    
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
        printf("client %i \n",num_successful_connections);
    }
    pthread_mutex_unlock(&mtx_connection_established);
    /* printf("Player: Unlocked conn \n"); */

    pthread_mutex_lock(&mtx_start_communicating); 
    while (num_successful_connections != -1) { // wait for start signal from main thread
        /* printf("Player: Unlocking conn and waiting for signal \n"); */ 
        pthread_cond_wait(&cond_start_signal, &mtx_start_communicating);
    }
        /* printf("Player: Done waiting, unlocking \n"); */
        pthread_mutex_unlock(&mtx_start_communicating);


    /* client_signal_main(); */
        
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
#ifndef BOOL_COMPRESS
    int elements_to_rec =  ((receiver_args*) threadParameters)->elements_to_rec[rounds];
    elements_to_rec = elements_to_rec * sizeof(DATATYPE);
#if USE_SSL == 1
            if ((receive_all(ssl, ((receiver_args*) threadParameters)->received_elements[rounds], &((receiver_args*) threadParameters)->elements_to_rec[rounds]) == -1))

#else
            if ((receive_all(sockfd, ((receiver_args*) threadParameters)->received_elements[rounds], &((receiver_args*) threadParameters)->elements_to_rec[rounds]) == -1))

                /* if ((recv(sockfd, ((char*) ((receiver_args*) threadParameters)->received_elements[rounds]), elements_to_rec, MSG_WAITALL)) == -1) */ 
#endif
            {
                perror("recv");
                printf("Failed to receive all bytes!\n"); 
                exit(1);
            }
#endif
#ifdef BOOL_COMPRESS
    int elements_to_rec =  (((receiver_args*) threadParameters)->elements_to_rec[rounds] + 7) / 8;
    uint8_t* rec_buffer = new (std::align_val_t(sizeof(uint64_t))) uint8_t[elements_to_rec];
#if USE_SSL == 1
            if ((receive_all(ssl, rec_buffer , &elements_to_rec)) == -1) {
    #else
            if ((recv(sockfd, rec_buffer , elements_to_rec, MSG_WAITALL)) == -1) 
    #endif
    {        
            perror("recv");
                exit(1);
            }
unpack(rec_buffer,((receiver_args*) threadParameters)->received_elements[rounds],((receiver_args*) threadParameters)->elements_to_rec[rounds]);
delete[] rec_buffer;
#endif
printf("received %i bytes from player %i in round %i out of %i \n", ((receiver_args*) threadParameters)->elements_to_rec[rounds], ((receiver_args*) threadParameters)->connected_to, rounds + 1, ((receiver_args*) threadParameters)->rec_rounds);
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
    
     

    // Clean up
    #if USE_SSL == 1
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    #endif
    close(sockfd);
    pthread_exit( NULL );
}
