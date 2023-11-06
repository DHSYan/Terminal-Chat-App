#include "lib.h"
#include "const.h"
#include "client-handler.h"
#include <stdio.h>

void* client_handler(void* client_info) {
    struct thread_info* thread_info = 
        (struct thread_info*) client_info;

    int connect_socket = thread_info->socket;
    int is_client_alive = false;

    int max_buffer_len = SMALL_BUF;
    char* buffer = malloc(sizeof(char)*SMALL_BUF);
    memset(buffer, 0, SMALL_BUF);

    int recv_res = recv(connect_socket, buffer, SMALL_BUF, 0);
    printf("%s", buffer);

    if (recv_res < 0) {
        perror("Something is wrong with listening to the handshake");
    }

    if (strstr(buffer, "SYN") != NULL) {
        int send_res = send(connect_socket, "[ACKSYN]|Welcome\n", SMALL_BUF, 0);
        if (send_res < 0 ) {
            perror("something's wrong with sending");
        }
        is_client_alive = true;
    } else {
        perror("client didin't send SYN");
    }

    return NULL;
}
