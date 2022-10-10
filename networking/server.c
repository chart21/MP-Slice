#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "sockethelper.h"

//#define PORT "6000"  // the port users will be connecting to

#define BACKLOG 1	 // how many pending connections queue will hold

int sendall(int s, DATATYPE *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len * sizeof(DATATYPE); // how many we have left to send
    int n_send;

    while(total < *len) {
        n_send = send(s, ((char*) buf)+total, bytesleft, 0);
        if (n_send == -1) { break; }
        total += n_send;
        bytesleft -= n_send;
    }

    *len = total; // return number actually sent here

    return n_send==-1?-1:0; // return -1 on failure, 0 on success
}

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}




void *sender(void* threadParameters)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
    
    char port[4];
    sprintf(port, "%d", ((sender_args*) threadParameters)->port);
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("Waiting for connections...\n");

	  // main accept() loop
		sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("Got connection from %s\n", s);
    
		/* if (!fork()) { // this is the child process */
		/* 	close(sockfd); // child doesn't need the listener */
		/* 	if (send(new_fd, "Hello, world!", 13, 0) == -1) */
		/* 		perror("send"); */
		/* 	close(new_fd); */
		/* 	exit(0); */
		/* } */
		
        pthread_mutex_lock(&mtx_connection_established);
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
    if(num_successful_connections == 2 * (((sender_args*) threadParameters)->player_count -1)) {
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

        while (rounds < ((sender_args*) threadParameters)->send_rounds) //continue until all data is sent
        {
            pthread_mutex_lock(&mtx_send_next); 
            while(rounds >= sending_rounds)
                    pthread_cond_wait(&cond_send_next, &mtx_send_next); //make sure that there is new data to send, singaled by main
            pthread_mutex_unlock(&mtx_send_next);
            //char buf[1024] = { 0 };
            //recv(new_fd, buf, 32, MSG_WAITALL);
            if(((sender_args*) threadParameters)->elements_to_send[rounds] > 0)
            {
                if (sendall(new_fd, ((sender_args*) threadParameters)->sent_elements[rounds], &((sender_args*) threadParameters)->elements_to_send[rounds]) == -1) {
                perror("sendall");
                printf("We only sent %d bytes because of the error!\n", ((sender_args*) threadParameters)->inputs_size);
            }
            }
                //Delete sent data
           //free(((thargs_p*) threadParameters)->sent_elements[rounds]);

            rounds += 1;
        

        } 
        //main thread should not be interested if data was sent, but only make new data available, increase rounds and signal
        /* pthread_mutex_lock(&mtx_data_sent); */
        /* sockets_sent += 1; */
        /* if(sockets_sent == ((thargs_t*) threadParameters)->num_players -1) */ 
        /*     pthread_cond_signal(&cond_data_sent); // signal main thread that sending is finished */
        /* pthread_mutex_unlock(&mtx_data_sent); */
        /* } */

        close(new_fd);  // parent doesn't need this
        shutdown(sockfd, SHUT_RDWR);
	    

		printf("Connection closed to %s\n", s);
	
pthread_exit( NULL );
}
