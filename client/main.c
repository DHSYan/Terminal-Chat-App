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


// #include "connect-to-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdbool.h>

#include "interaction.h"


int main(int argc, char* argv[]) {
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    const char* server_addr = "127.0.0.1";
    const char* server_port = argv[1];

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

    /////////////////////////////////////////////////////////
    
    char* command = malloc(sizeof(char)*100);
    char* buffer = malloc(sizeof(char)*100);
    char* handshake = malloc(sizeof(char)*100);
    strcpy(handshake, "[client] Hello");

    
    send(handshake_socket, handshake, strlen(handshake)+1, 0);
    while (true) {
        recv(handshake_socket, buffer, 100, 0);
        printf("%s", buffer);
        fgets(command, 99, stdin);
        printf("we scanned %s\n", command);
        send_command(handshake_socket, command);
    }

    return 0;
}

