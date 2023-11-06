#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "lib.h"

// This will contains the informations that all thread_user will need.
// Or the information that the server needs to run properly
typedef struct global_info {
    FILE* userlog;
    FILE* messagelog;
    // user* valid_users;
    int active_user_seq_num;
} global_info;

// This will contain all the inforamiot that is specific to the client
// since each client is it own thread.
typedef struct thread_info {
    global_info* global_info;
    int socket;
    char* addr;
    int port;
    // user* thread_user;
} thread_info;

void* client_handler(void* client_info);
#endif

