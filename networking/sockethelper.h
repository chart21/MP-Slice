#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
pthread_mutex_t mtx_connection_established;
pthread_mutex_t mtx_start_communicating;
pthread_cond_t cond_successful_connection;
int num_successful_connections = 0;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

typedef struct thread_arguments {
  int num_players;
  int threadID;
  char *inputs;
  int inputs_size;
  char *ip;
  int port;
  char *hostname;
  //char *data;
  //char *length
} thargs_t;

typedef struct player_arguments {
  char *inputs;
  int inputs_size;
  int port;
} thargs_p;


