#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "lib.h"
// #include "auth.h"

// This will contains the informations that all thread_user will need.
// Or the information that the server needs to run properly
typedef struct global_info {
    FILE* userlog;
    FILE* messagelog;
    struct user* valid_users;
    int active_user_seq_num;
    int message_seq_sum;
} global_info;

// This will contain all the inforamiot that is specific to the client
// since each client is it own thread.
typedef struct thread_info {
    struct global_info* global_info;
    int socket;
    char* addr;
    int port;
    struct user* thread_user;
    int udp_port;
} thread_info;

void* client_handler(void* client_info);
#endif

