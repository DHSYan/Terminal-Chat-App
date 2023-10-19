#include "TCPServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct sockaddr_in server_addr;

int init_server() {
    const char* port = "3000";

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

    while(true) {

        struct sockaddr_storage client_addr;
        socklen_t client_addr_size;

        int connect_socket = 
            accept(handshake_socket,
                    (struct sockaddr*) &client_addr,
                    &client_addr_size);
        if (connect_socket < 0) {
            perror("Something went wrong the accepting");
        }

        int max_buffer_len = 10000;
        char buffer[max_buffer_len];
        // char* buffer = malloc(sizeof(char) * max_buffer_len);
        memset(buffer, 0, strlen(buffer) * sizeof(char));

        int recv_res;

        while ((recv_res =
                    recv(connect_socket, buffer, max_buffer_len, 0)) > 0) {
            // recv_res = recv(connect_socket, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
            
            // Taken this from Youtube Video by Jacob Sorber
            if (buffer[recv_res-1] == '\n') {
                break;
            }

            memset(buffer, 0, strlen(buffer)*sizeof(char));
        }

        close(connect_socket);

    }

    close(handshake_socket);
    freeaddrinfo(res);

    return 0;
}
