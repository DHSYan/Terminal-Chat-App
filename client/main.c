// This is a chat app program coded for the Course COMP3331 at UNSW
// This is the client app
//
// Author: Sean Yan
// zID: z5521807
//
// Acknowledegment
// All Code borrowed or taken from other resources will be comment before the 
// function or block section
//
//


#include "connect-to-server.h"
#include "auth.h"
// Stdlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main() {
    struct user* valid_users = load_credentials();

    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    const char* server_addr = "127.0.0.1";
    const char* server_port = "3000";

    int get_addr_res = getaddrinfo(server_addr, server_port, &hints, &res);
    
    int handshake_socket =
        socket(res->ai_family,
                res->ai_socktype,
                res->ai_protocol);
    if (handshake_socket == -1) { printf("socket\n"); }

    int connect_res = connect(handshake_socket, res->ai_addr, res->ai_addrlen);
    if (connect_res < 0) { 
        perror("Couldn't Connect to server\n");
    }

    while(true) {

        int buffer_max_len = 100;
        char buffer[buffer_max_len];
        /* printf("What do you want to send? "); */
        // scanf("%10000s", buffer);
        /* fgets(buffer, 100, stdin); */
        strcpy(buffer, "init\n");
        int send_res = send(handshake_socket, buffer, strlen(buffer)+1, 0); 
        /* printf("We send %d, %s\n", send_res, buffer); */


        // Waiting for server's response
        int recv_buffer_max_len = 100;
        char recv_buffer[recv_buffer_max_len];
        recv(handshake_socket, recv_buffer, recv_buffer_max_len, 0);

        char isCont[50];
        scanf("%49s", isCont);
    }
    
    return 0;
}

