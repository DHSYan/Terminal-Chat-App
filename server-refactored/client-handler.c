#include "lib.h"
#include "const.h"
#include "client-handler.h"
#include "protocol-processor.h"
#include <stdlib.h>

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

    int client_connected = -1;

    if (strstr(buffer, "SYN") != NULL) {
        char* parsed = strtok(buffer, "|");
        parsed = strtok(NULL, "|");
        printf("The UDP Port of this therad is %s\n", parsed);
        thread_info->udp_port = atoi(parsed);

        int send_res = send(connect_socket, "[SYNACK]|Welcome\n", SMALL_BUF, 0);
        if (send_res < 0 ) {
            perror("something's wrong with sending");
        }

        // Wait for ACK 
        memset(buffer, 0, SMALL_BUF);
        recv(connect_socket, buffer, SMALL_BUF, 0);
        if (strstr(buffer, "ACK") != NULL) {
            client_connected = 0;
        } else {
            perror("client didn't send ACK");
        }
    } else {
        perror("client didin't send SYN");
    }

    while (client_connected == 0) {
        // listen for message 
        // pass the message to the protocol processer
        memset(buffer, 0, SMALL_BUF);
        sleep(1); // this could cause problem in CSE
        send(connect_socket,
            "[input]|Enter Command (/msgto, /activeuser, /creategroup, /joingroup, /groupmsg, /p2pvideo ,/logout): \n",
            SMALL_BUF,
            0);
        recv(connect_socket, buffer, SMALL_BUF, 0);

        client_connected = system_caller(buffer, thread_info);
    
        if (client_connected == -1) {
            send(connect_socket,
                    "[FIN]|Bye, Thank you using 3331ChatApp\n",
                    SMALL_BUF,
                    0);
            close(connect_socket);
        }
    }


    return NULL;
}
