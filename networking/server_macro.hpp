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
#else
#define SOCKET_TYPE int
#endif
#include "sockethelper.h"
#ifndef BOOL_COMPRESS
#define DTYPE DATATYPE
#else
#define DTYPE uint8_t
#endif

#if USE_SSL == 1
SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM <= 0 )) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}
#endif


int create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

        int yes = 1; // reuse address 
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}



    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return s;
}



int sendall(SOCKET_TYPE s, DTYPE *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len * sizeof(DATATYPE); // how many we have left to send
    int n_send;

    while(total < *len) {
        #if USE_SSL == 1
        n_send = SSL_write(s, ((char*) buf)+total, bytesleft);
        #else
        n_send = send(s, ((char*) buf)+total, bytesleft, 0);
        #endif
        if (n_send == -1) { break; }
        total += n_send;
        bytesleft -= n_send;
    }

    *len = total; // return number actually sent here

    return n_send==-1?-1:0; // return -1 on failure, 0 on success
}

void *sender(void* threadParameters)
{
    // Create a socket
    int sockfd;
    #if USE_SSL == 1
    SSL_library_init();
    SSL_CTX* ctx = create_context();
    configure_context(ctx);
    /* SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE); */
    #endif



    /* char port[4]; */
    /* sprintf(port, "%d", ((sender_args*) threadParameters)->port); */

    sockfd = create_socket(((sender_args*) threadParameters)->port);
    // Bind the socket


    // Accept the first connection
    sockaddr_in client_addr = {};
    socklen_t addr_len = sizeof(client_addr);
    int clientfd = accept(sockfd, (sockaddr*)&client_addr, &addr_len);
    if (clientfd == -1) {
        std::cerr << "Failed to accept connection\n";
        exit(1);
    }
printf("Connected to Player %i\n", ((sender_args*) threadParameters)->connected_to);
    

   
    // Send data to the client
    #if USE_SSL == 1
    auto ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientfd);
    if (SSL_accept(ssl) <= 0) {
    throw std::runtime_error("Error performing SSL handshake");
    }    
#endif



        pthread_mutex_lock(&mtx_connection_established);
        num_successful_connections += 1; 
    if(num_successful_connections == 2 * (((sender_args*) threadParameters)->player_count -1)) {
        pthread_cond_signal(&cond_successful_connection); //signal main thread that all threads have connected
        printf("server %i \n",num_successful_connections);
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
    
        int rounds = 0;

        while (rounds < ((sender_args*) threadParameters)->send_rounds) //continue until all data is sent
        {
            pthread_mutex_lock(&mtx_send_next); 
            while(rounds >= sending_rounds)
                    pthread_cond_wait(&cond_send_next, &mtx_send_next); //make sure that there is new data to send, singaled by main
            pthread_mutex_unlock(&mtx_send_next);
            //char buf[1024] = { 0 };
            //recv(new_fd, buf, 32, MSG_WAITALL);
            /* printf("sending round %i out of %i \n", rounds, ((sender_args*) threadParameters)->send_rounds); */
            if(((sender_args*) threadParameters)->elements_to_send[rounds] > 0)
            {
#ifndef BOOL_COMPRESS
            int elements_to_send =  ((sender_args*) threadParameters)->elements_to_send[rounds];
            elements_to_send = elements_to_send * sizeof(DATATYPE);
#if USE_SSL == 1
                if (sendall(ssl, ((sender_args*) threadParameters)->sent_elements[rounds], &((sender_args*) threadParameters)->elements_to_send[rounds]) == -1) 
#else
                if (sendall(clientfd, ((sender_args*) threadParameters)->sent_elements[rounds], &((sender_args*) threadParameters)->elements_to_send[rounds]) == -1) 
#endif
                {
                    perror("sendall");
                printf("Failed to send all bytes!\n"); 
            }
                //delete Arr
#endif
#ifdef BOOL_COMPRESS
            int elements_to_send =  (((sender_args*) threadParameters)->elements_to_send[rounds] + 7)/8;
            uint8_t* send_buf = new (std::align_val_t(sizeof(uint64_t))) uint8_t[elements_to_send];
pack(((sender_args*) threadParameters)->sent_elements[rounds],send_buf,((sender_args*) threadParameters)->elements_to_send[rounds]);
#if USE_SSL == 1
                if (sendall(clientfd, send_buf, &elements_to_send) == -1)
#else
                if (sendall(ssl, send_buf, &elements_to_send) == -1)
#endif

                {
                perror("sendall");
                printf("Failed to send all bytes!\n"); 
            }
                /* delete[] send_buf; */
                //delete Arr
#endif
            printf("sent %i bytes to player %i in round %i out of %i \n", ((sender_args*) threadParameters)->elements_to_send[rounds], ((sender_args*) threadParameters)->connected_to, rounds + 1, ((sender_args*) threadParameters)->send_rounds);
            }
                //Delete sent data
           //free(((thargs_p*) threadParameters)->sent_elements[rounds]);

            rounds += 1;
            /* printf("Next sending round \n"); */
        

        }






// Clean up
#if USE_SSL == 1
SSL_shutdown(ssl);
SSL_free(ssl);
SSL_CTX_free(ctx);
#endif
close(clientfd);
close(sockfd);
shutdown(sockfd, SHUT_RDWR);

printf("Connection closed to Player %i\n", ((sender_args*) threadParameters)->connected_to);
pthread_exit( NULL );

}
