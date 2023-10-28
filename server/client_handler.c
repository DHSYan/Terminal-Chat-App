#include "stdlib.h"

#include "client_handler.h"
#include "interaction.h"

void* client_handler(void* client_info) {
    printf("\n\n------------------New Client-------------------\n\n");
    struct client_thread_info* client = 
        (struct client_thread_info*) client_info;

    int connect_socket = client->socket;
    struct user* valid_user = client->valid_user;
    int is_client_alive = true;
    
    int max_buffer_len = 1024;
    char* buffer = malloc(sizeof(char)*max_buffer_len);
    // memset(buffer, 0, strlen(buffer)+1);
    
    int recv_res;
    while ((recv_res =
                recv(connect_socket, buffer, max_buffer_len, 0)) > 0) {
        /* int recv_res = recv(connect_socket, buffer, sizeof(buffer), 0); */
        printf("we got the init msg: %s\n", buffer);

        if (recv_res == -1) {
            puts("something is wrong with recv in server");
            exit(0);
        } else if (recv_res == 0) {
            puts("they disconnected");
            is_client_alive = false;
        }     

        listen_command(client->valid_user, connect_socket, buffer);
        memset(buffer, 0, strlen(buffer)+1);
    }
    close(connect_socket);
    return NULL;
}
