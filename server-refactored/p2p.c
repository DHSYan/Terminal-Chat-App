#include "client-handler.h"
#include "lib.h"
#include "const.h"
#include "p2p.h"
#include "auth.h"
#include <stdio.h>
#include <sys/socket.h>

// "[P2P]|addr port filename caller_username"
int p2p(char* msg, thread_info* thread_info) {
    int socket = thread_info->socket;

    char caller_username[SMALL_BUF];
    strcpy(caller_username, thread_info->thread_user->username);

    char* parsed = strtok(msg, " ");
    parsed = strtok(NULL, " "); // get rid of /p2pvideo

    char receiver_username[SMALL_BUF];
    strcpy(receiver_username, parsed);
    user* receiver = 
        return_user(receiver_username, thread_info->global_info->valid_users);
    char* addr = receiver->addr;
    int port  = receiver->udp_port;
    parsed = strtok(NULL, " "); // get rid of usernaem

    char filename[SMALL_BUF];
    strcpy(filename, parsed);


    char response[SMALL_BUF];
    sprintf(response,
            "[P2P]|%s %d %s %s\n",
            addr, port, filename, caller_username);
    send(socket, response, SMALL_BUF, 0);
    return 0;
}

