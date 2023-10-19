// I have referred back to Lab 3 for the implementation for a TCP Server 
// 
// I have referred to https://beej.us/guide/bgnet/html/#structs 
// for the implementation of the networking aspect
//
// I have referred to the Course Notes and Example for client and server
// impelementation

#include "connect-to-server.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int connect_to_server() {
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

    int buffer_max_len = 10000;
    char buffer[buffer_max_len];
    printf("What do you want to send? ");
    // scanf("%10000s", buffer);
    fgets(buffer, 10000, stdin);
    int send_res = send(handshake_socket, buffer, strlen(buffer)+1, 0); 
    printf("We send %d, %s\n", send_res, buffer);

    
    return 0;

}
