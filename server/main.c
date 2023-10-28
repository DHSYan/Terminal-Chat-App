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
#include "logging.h"
#include "stdlib.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>
// #include <pthread.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <sys/types.h>
// #include <netdb.h>
// #include <unistd.h>

// My Own Modules
#include "auth.h"
#include "interaction.h"
#include "client_handler.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    struct user* valid_user = load_credentials(atoi(argv[2]));

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

    struct global_info* global_info = malloc(sizeof(struct global_info));
    global_info->seq_num = 0; 
    global_info->valid_user = valid_user;

    // create the loggin files
    FILE* userlog = init_logging("userlog.txt");
    global_info->userlog = userlog;

    while(true) {

        struct sockaddr_storage client_addr;
        socklen_t client_addr_size;

        int connect_socket = accept(handshake_socket,
                                    (struct sockaddr*) &client_addr,
                                    &client_addr_size);
        if (connect_socket < 0) {
            perror("Something went wrong the accepting");
        }

        global_info->seq_num++; // Increase on each connection

        struct thread_info* thread_info = malloc(sizeof(struct thread_info));

        thread_info->socket = connect_socket;
        thread_info->global_info = global_info;

        /* Reference:
            // The casting information was taken from Beej's Guide in 
            // Section 6.2
            // and got the idea of inet_ntoa from course example
        */
        thread_info->addr = 
            inet_ntoa((((struct sockaddr_in*)&client_addr)->sin_addr));
        thread_info->port =
            ntohs(((struct sockaddr_in*)&client_addr)->sin_port);
        
        pthread_create(
                &client_thread,
                NULL,
                client_handler,
                (void*) thread_info);
    }

    close(handshake_socket);

    freeaddrinfo(res);
    return 0;
}



// References
