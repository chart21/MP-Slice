#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

#include "sockethelper.h"
using namespace std;

void *receiver(void* threadParameters){
    
    // Initialize OpenSSL library
    SSL_library_init();
    SSL_load_error_strings();

    // Create SSL context
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx) {
        cerr << "Error creating SSL context" << endl;
        exit(1);
    }

    // Create SSL connection
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        cerr << "Error creating SSL connection" << endl;
        SSL_CTX_free(ctx);
        exit(1);
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error creating socket" << endl;
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        exit(1);
    }

    // Get server address
    struct hostent *server = gethostbyname("example.com");
    if (!server) {
        cerr << "Error resolving hostname" << endl;
        close(sockfd);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        exit(1);
    }

    // Fill in server address structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(443);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    while (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        /* cerr << "Error connecting to server" << endl; */
        /* sleep(1); // Wait for 1 second before trying again */
    }

    // Attach SSL connection to socket
    SSL_set_fd(ssl, sockfd);

    // Perform SSL handshake
    if (SSL_connect(ssl) < 0) {
        cerr << "Error performing SSL handshake" << endl;
        SSL_shutdown(ssl);
        close(sockfd);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        exit(1);
    }

    
    client_signal_main();

    // Send and receive data over SSL connection
    // ...

    // Clean up
    SSL_shutdown(ssl);
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}

