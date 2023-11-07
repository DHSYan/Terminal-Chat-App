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

#define SMALL_BUF 100

struct global_info {
    bool status; 
};
typedef struct global_info global_info;

struct server_message {
    char message[1024];
    int socket;
    global_info* global_info;
};

void* response(void* server_message) {
    struct server_message* message = (struct server_message*) server_message;
    char* send_buffer = malloc(sizeof(char) * SMALL_BUF);
    char* actual_message = strchr(message->message, '|');

    if (actual_message == NULL) {
        printf("Server sent an invalid message with invalid format\n");
        printf("\n\n\nServer sent: %s\n\n\n", actual_message);
        message->global_info->status = false;
        return NULL;
    } 
    // else {
        // printf("Server response:\n  %s ", actual_message);
        // printf("%s", actual_message);
        // if (strstr(message->message, "[input]") != NULL) {
        //     fgets(send_buffer, 99, stdin);
        //     send(message->socket, send_buffer, SMALL_BUF, 0);
        // } else if (strstr(message->message, "[FIN]") != NULL) {
        //     printf("You have logout or server sent FIN\n");
        //     message->global_info->status = false;
        //     return NULL;
        // }
    // }
    // printf("Server response:\n  %s ", message->message);
    printf("%s", actual_message);
    if (strstr(message->message, "[input]") != NULL) {
        fgets(send_buffer, 99, stdin);
        send(message->socket, send_buffer, SMALL_BUF, 0);
    } else if (strstr(message->message, "[FIN]") != NULL) {
        printf("You have logout or server sent FIN\n");
        message->global_info->status = false;
    }


    return NULL;
}
// void* response(void* server_message) {
//     struct server_message* message = (struct server_message*) server_message;
//     // char* mymessage = message->message;
//     char* send_buffer = malloc(sizeof(char) * SMALL_BUF);
//     // if (strstr(message->message, "command") != NULL) {
//     //     printf("Enter command: ");
//     //     fgets(send_buffer, 99, stdin);
//     //     send(message->socket, send_buffer, SMALL_BUF, 0);
//     // } else {
//     //     printf("Server response:\n  %s ", message->message);
//     //     fgets(send_buffer, 99, stdin);
//     //     send(message->socket, send_buffer, SMALL_BUF, 0);
//     // }
//     char* actual_message = strchr(message->message, '|');
//     if (actual_message == NULL) {
//         printf("Server sent an invalid message with invalid format\n");
//         printf("\n\n\nServer sent: %s\n\n\n", actual_message);
//         return NULL;
//     } else {
//         // printf("Server response:\n  %s ", actual_message);
//         printf("%s", actual_message);
//     }
//
//     if (strstr(message->message, "[input]") != NULL) {
//         fgets(send_buffer, 99, stdin);
//         send(message->socket, send_buffer, SMALL_BUF, 0);
//     } else if (strstr(message->message, "[FIN]") != NULL) {
//         printf("You have logout or server sent FIN\n");
//         // close(message->socket);
//         // message->connection_status = false;
//         return NULL;
//     } else {
//         // printf("Ok");
//         // send(message->socket, "OK", SMALL_BUF, 0);
//     }
//
//     return NULL;
// }


int main(int argc, char* argv[]) {
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    const char* server_addr = "127.0.0.1";
    const char* server_port = argv[1];

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
    
    char* command = malloc(sizeof(char)*100);
    char* recv_buffer = malloc(sizeof(char)*100);
    char* handshake = malloc(sizeof(char)*100);
    char* send_buffer = malloc(sizeof(char)*100);
    strcpy(handshake, "[client][SYN]|Hello\n");

    
    int init_handshack = 
        send(handshake_socket, handshake, strlen(handshake)+1, 0);

    // printf("\n//////////////////Phase 3, wating for SYN ACK//////////////////\n");
    memset(recv_buffer, 0, SMALL_BUF);
    int server_handshake_res = recv(handshake_socket, recv_buffer, 100, 0);

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
    // while (recv(handshake_socket, recv_buffer, 1000, 0) > 0) {
    while (global_info->status == true) {
    // while (true) {
        // memset(recv_buffer, 0, 100);
        recv(handshake_socket, recv_buffer, 100, 0);
    
        struct server_message* message = malloc(sizeof(struct server_message));
        message->global_info = global_info;
        strcpy(message->message, recv_buffer);
        memset(recv_buffer, 0, 100);
        message->socket = handshake_socket;
       
        pthread_create(&response_thread, NULL, response, (void*) message);
    }
    // struct server_message* message = malloc(sizeof(struct server_message));
    //
    // while (recv(handshake_socket, recv_buffer, 1000, 0) > 0) {
    //
    //     strcpy(message->message, recv_buffer);
    //     memset(recv_buffer, 0, SMALL_BUF);
    //
    //     message->socket = handshake_socket;
    //
    //     pthread_create(&response_thread, NULL, response, (void*) message);
    // }
    //
    return 0;
}

