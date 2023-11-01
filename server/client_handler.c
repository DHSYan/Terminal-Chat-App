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
    // memset(buffer, 0, strlen(buffer)+1);
    
    int recv_res = recv(connect_socket, buffer, max_buffer_len, 0);
    // while ((recv_res =
    //             recv(connect_socket, buffer, max_buffer_len, 0)) > 0) {
        /* int recv_res = recv(connect_socket, buffer, sizeof(buffer), 0); */
    printf("we got the init msg: %s\n", buffer);

    if (recv_res == -1) {
        puts("something is wrong with recv in server");
        exit(0);
    } 
    // else if (recv_res == 0) {
    //     puts("they disconnected");
    //     is_client_alive = false;
    // } 
    else {
        is_client_alive = true;
        printf("\n\n--------------entering loop of listening to commands------------------\n");
    }
    
    while (is_client_alive) {
        printf("promting command...\n");
        listen_command(thread_info, connect_socket, buffer);
        memset(buffer, 0, strlen(buffer)+1);
    }

    close(connect_socket);
    return NULL;
}
