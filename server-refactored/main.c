#include "lib.h"

#include "auth.h"
#include "client-handler.h"
#include "logging.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: "
                "./server sever_port number_of_consecutive_failed_attempts\n");
        return -1;
    }
    int max_attempt = atoi(argv[2]);
    if (max_attempt < 1 || max_attempt > 5) {
        printf("Invalid number of allowed failed consecutive attempt: number. "
                "The valid value of argument number is "
                "an integer between 1 and 5\n");
        return -1;
    }
    
    user* valid_users = load_credentials(max_attempt);

    printf("\n\nInitializing Server..\n\n");
    const char* port = argv[1];

    // Creating Server Socket
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

    global_info* global_info = malloc(sizeof(struct global_info));
    global_info->active_user_seq_num = 0;
    global_info->message_seq_sum = 0;
    global_info->valid_users = valid_users;
    global_info->userlog = init_logging("userlog.txt");
    // global_info->messagelog = init_logging("messagelog.txt");

    while(true) {
        struct sockaddr_storage client_addr;
        socklen_t client_addr_size;
        int connect_socket = accept(handshake_socket,
                                    (struct sockaddr*) &client_addr,
                                    &client_addr_size);
        if (connect_socket < 0) {
            perror("Something went wrong the accepting");
        }

        thread_info* thread_info = malloc(sizeof(struct thread_info));
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

