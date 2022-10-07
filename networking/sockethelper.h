#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../arch/DATATYPE.h"
pthread_mutex_t mtx_connection_established;
pthread_mutex_t mtx_start_communicating;
pthread_cond_t cond_successful_connection;
int num_successful_connections = 0;


//int sending_rounds = 0;
int receiving_rounds = 0;
pthread_mutex_t mtx_receive_next;
pthread_cond_t cond_receive_next;

int* sockets_received;
pthread_mutex_t mtx_data_received;
pthread_cond_t cond_data_received;

    
int sending_rounds = 0;
pthread_mutex_t mtx_send_next;
pthread_cond_t cond_send_next;

int sockets_sent = 0;
pthread_mutex_t mtx_data_sent;
pthread_cond_t cond_data_sent;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

typedef struct thread_arguments {
  int player_count;
  int player_id;
  int connected_to;
  DATATYPE **received_elements;
  int inputs_size;
  char *ip;
  int port;
  char *hostname;
  int rec_rounds;
  int *elements_to_rec;
  //char *data;
  //char *length
} thargs_t;

typedef struct player_arguments {
  DATATYPE **sent_elements;
  int inputs_size;
  int port;
  int player_id;
  int connected_to;
  int send_rounds;
  int *elements_to_send;
  //char *data;
} thargs_p;
