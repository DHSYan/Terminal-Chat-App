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
#include <pthread.h>
#include <unistd.h>

#include "interaction.h"

#define SMALL_BUF 2048

struct global_info {
    bool status; 
};
typedef struct global_info global_info;

struct server_message {
    char message[SMALL_BUF];
    int socket;
    global_info* global_info;
};

void* response(void* server_message) {
    struct server_message* message = (struct server_message*) server_message;
    char* send_buffer = malloc(sizeof(char) * SMALL_BUF);
    char* actual_message = strchr(message->message, '|');

    if (actual_message == NULL) {
        message->global_info->status = false;
        return NULL;
    } 
    printf("\n%s", actual_message);
    if (strstr(message->message, "[input]") != NULL) {
        fgets(send_buffer, 99, stdin);
        send(message->socket, send_buffer, SMALL_BUF, 0);
    } else if (strstr(message->message, "[FIN]") != NULL) {
        message->global_info->status = false;
    }


    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("usage: ./client server_IP server_port client_udp_server_port\n");
        return -1;
    }
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    const char* server_addr = argv[1];
    const char* server_port = argv[2];
    const char* my_udp_port = argv[3];

    int get_addr_res = getaddrinfo(server_addr, server_port, &hints, &res);
    
    // printf("\n//////////////////Phase 1, creating the socket and connecting//////////////////\n");

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
    // printf("\n//////////////////Phase 2, sending handshake//////////////////\n");
    
    char* command = malloc(sizeof(char)*SMALL_BUF);
    char* recv_buffer = malloc(sizeof(char)*SMALL_BUF);
    char* handshake = malloc(sizeof(char)*SMALL_BUF);
    char* send_buffer = malloc(sizeof(char)*SMALL_BUF);
    strcpy(handshake, "[client][SYN]|");
    strcat(handshake, my_udp_port);
    strcat(handshake, "\n");

    
    int init_handshack = 
        send(handshake_socket, handshake, strlen(handshake)+1, 0);

    // printf("\n//////////////////Phase 3, wating for SYN ACK//////////////////\n");
    memset(recv_buffer, 0, SMALL_BUF);
    int server_handshake_res = recv(handshake_socket, recv_buffer, SMALL_BUF, 0);

    int allow_to_run = false;
    
    global_info* global_info = malloc(sizeof(struct global_info));
    
    if (strstr(recv_buffer, "[SYNACK]") != NULL) {
        global_info->status = true;
        memset(recv_buffer, 0, SMALL_BUF);
        send(handshake_socket, "[ACK]\n", SMALL_BUF, 0);
        recv(handshake_socket, recv_buffer, SMALL_BUF, 0); // wait for server to send the comm
        send(handshake_socket, "/login\n", SMALL_BUF, 0);
    } else {
        printf("No ACK from Server\n");
        return -1;
    }

    pthread_t response_thread;


    // printf("\n//////////////////Phase 4, Staring the loop//////////////////\n");
    while (global_info->status == true) {
        recv(handshake_socket, recv_buffer, SMALL_BUF, 0);
    
        struct server_message* message = malloc(sizeof(struct server_message));
        message->global_info = global_info;
        strcpy(message->message, recv_buffer);
        memset(recv_buffer, 0, SMALL_BUF);
        message->socket = handshake_socket;
       
        pthread_create(&response_thread, NULL, response, (void*) message);
    }
    return 0;
}

