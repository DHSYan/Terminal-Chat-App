#include "lib.h"
#include "const.h"
#include "client-handler.h"
#include "protocol-processor.h"

void* client_handler(void* client_info) {
    struct thread_info* thread_info = 
        (struct thread_info*) client_info;

    int connect_socket = thread_info->socket;

    int max_buffer_len = SMALL_BUF;
    char* buffer = malloc(sizeof(char)*SMALL_BUF);
    memset(buffer, 0, SMALL_BUF);

    int recv_res = recv(connect_socket, buffer, SMALL_BUF, 0);

    if (recv_res < 0) {
        perror("Something is wrong with listening to the handshake");
    }

    int client_connected = false;

    if (strstr(buffer, "SYN") != NULL) {
        int send_res = send(connect_socket, "[ACKSYN]|Welcome\n", SMALL_BUF, 0);
        if (send_res < 0 ) {
            perror("something's wrong with sending");
        }

        // Wait for ACK 
        memset(buffer, 0, SMALL_BUF);
        recv(connect_socket, buffer, SMALL_BUF, 0);
        if (strstr(buffer, "ACK") != NULL) {
            client_connected = true;
        } else {
            perror("client didn't send ACK");
        }
    } else {
        perror("client didin't send SYN");
    }

    while (client_connected) {
        // listen for message 
        // pass the message to the protocol processer
        memset(buffer, 0, SMALL_BUF);
        send(connect_socket, "|Enter Command: ", SMALL_BUF, 0);
        recv(connect_socket, buffer, SMALL_BUF, 0);

        client_connected = system_caller(buffer);
    }

    return NULL;
}
