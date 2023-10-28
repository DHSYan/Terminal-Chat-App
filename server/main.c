// This is a chat app program coded for the Course COMP3331 at UNSW.
// this is the server app
//
// Author: Sean Yan
// zID: z5521807
//
// Acknowledegment
// All Code borrowed or taken from other resources will be comment before the 
// function or block section
//
//


// Stdlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

// My Own Modules
#include "auth.h"
/* #include "TCPServer.h" */


#define SMALL_BUF 100

struct client_thread_info {
    int socket;
    struct user* valid_user;
};

void send_login(int socket) {
    printf("Hello world\n");
    send(socket, "hello world", 11, 0);
}

void listen_command(int socket, char* command) {
    char prompt[20] = "[server] Command: ";
    send(socket, prompt, sizeof(prompt), 0);

    char buffer[SMALL_BUF]; 
    memset(buffer, 0, sizeof(buffer));
    printf(" |  Listening for commands...\n");
    int recv_res = recv(socket, buffer, sizeof(buffer), 0);

    if (recv_res < 0) {
        printf("Listing for command failed to recv\n");
    }

    char* function = malloc(sizeof(char)*SMALL_BUF);
    char* substring = strstr(buffer, "command:");

    // If the command: string don't exist
    // then it is not a command, ignore it
    if (substring == NULL) {
        return;
    } else if (strstr(buffer, "login") != NULL) {
        printf("   |command is login, sending it...\n");
        send_login(socket);
    } else {
        printf("What is this command? '%s'\n", buffer);
    }
}


void* client_handler(void* client_info) {
    printf("\n\n------------------New Client-------------------\n\n");
    struct client_thread_info* client = 
        (struct client_thread_info*) client_info;

    int connect_socket = client->socket;
    struct user* valid_user = client->valid_user;
    int is_client_alive = true;
    
    int max_buffer_len = 1024;
    char* buffer = malloc(sizeof(char)*max_buffer_len);
    memset(buffer, 0, strlen(buffer));
    
    int recv_res;
    while ((recv_res =
                recv(connect_socket, buffer, strlen(buffer), 0)) > 0) {
        /* int recv_res = recv(connect_socket, buffer, sizeof(buffer), 0); */
        printf("we got the init msg: %s\n", buffer);

        if (recv_res == -1) {
            puts("something is wrong with recv in server");
            exit(0);
        } else if (recv_res == 0) {
            puts("they disconnected");
            is_client_alive = false;
        }     

        listen_command(connect_socket, buffer);
        memset(buffer, 0, strlen(buffer));
    }
    close(connect_socket);
    return NULL;
}

// This has to interact with the client
int main(int argc, char* argv[]) {
    struct user* valid_user = load_credentials();

    printf("\n\nInitializing Server..\n\n");
        
    const char* port = argv[1];

    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int get_addr_res = getaddrinfo(NULL, port, &hints, &res);
    if (get_addr_res != 0) {
        perror("Something went wrong with getting the address of localhost");
    }

    int handshake_socket = 
        socket(res->ai_family,
                res->ai_socktype,
                res->ai_protocol);
    if (handshake_socket == -1) { perror("socket"); }

    int bind_res = bind(handshake_socket, res->ai_addr, res->ai_addrlen);
    if (bind_res == -1) {
        perror("Something went wrong with binding the port");
    }

    int queue_num = 10;
    int listen_res = listen(handshake_socket, queue_num);

    pthread_t client_thread;

    while(true) {

        struct sockaddr_storage client_addr;
        socklen_t client_addr_size;

        int connect_socket = accept(handshake_socket,
                                    (struct sockaddr*) &client_addr,
                                    &client_addr_size);
        if (connect_socket < 0) {
            perror("Something went wrong the accepting");
        }
            
        struct client_thread_info* client_info =
            malloc(sizeof(struct client_thread_info));
        client_info->socket = connect_socket;
        client_info->valid_user = valid_user;
        
        pthread_create(
                &client_thread,
                NULL,
                client_handler,
                (void*) client_info);
    }

    close(handshake_socket);

    freeaddrinfo(res);
    return 0;
}



// References
