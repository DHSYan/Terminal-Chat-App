#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <stdio.h>
struct thread_info {
    struct global_info* global_info;
    int socket;
    char* addr;
    int port;
    struct user* thread_user;

};

struct global_info {
    FILE* userlog;
    FILE* messagelog;
    struct user* valid_user;
    // struct group* groups;
    int seq_num;
};

void* client_handler(void* client_info);

#endif
