#include "stdlib.h"

#include "client_handler.h"
#include "interaction.h"

void* client_handler(void* client_info) {
    printf("\n\n------------------New Client-------------------\n\n");
    struct thread_info* thread_info = 
        (struct thread_info*) client_info;

    printf("    | addr: %s | port: %d\n----------------------\n", 
            thread_info->addr, thread_info->port);

    int connect_socket = thread_info->socket;
    struct user* valid_user = thread_info->global_info->valid_user;
    int is_client_alive = false;
    
    int max_buffer_len = 1024;
    char* buffer = malloc(sizeof(char)*max_buffer_len);
    memset(buffer, 0, strlen(buffer)+1);
    
    int recv_res = recv(connect_socket, buffer, max_buffer_len, 0);
    printf("we got the init msg: %s\n", buffer);

    if (recv_res == -1) {
        printf("No init msg\n");
        // exit(0);
    } 
    // else if (recv_res == 0) {
    //     puts("they disconnected");
    //     is_client_alive = false;
    // } 
    else {
        int send_res = send(connect_socket, "[ACKSYN]|Welcome", SMALL_BUF, 0);
        if (send_res < 0 ) {
            perror("something's wrong with sending");
        }
        is_client_alive = true;
        printf("\n\n--------------entering loop of listening to commands------------------\n");
    }
    
    memset(buffer, 0, strlen(buffer)+1);
    while (is_client_alive) {
    // while(recv(connect_socket, buffer, max_buffer_len, 0) > 0) {
        printf("promting command...\n");
        int client_status = listen_command(thread_info, connect_socket, buffer);
        if (client_status == 1) {
            is_client_alive = false;
        } else {
            memset(buffer, 0, strlen(buffer)+1);
        }
    }

    // close(connect_socket);
    return NULL;
}
