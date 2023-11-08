#ifndef MSGTO_H
#define MSGTO_H

#include "client-handler.h"
#include "const.h"
#include "auth.h"
struct message {
    char username[SMALL_BUF];
    char msg[SMALL_BUF];
    int socket;
};

struct message* create_message(char* string, thread_info* thread_info);

struct message* better_create_message(
        char* username,
        char* groupname,
        char* msg,
        thread_info* thread_info);
void send_message(struct message* message, thread_info* thread_info);
int msgto(thread_info* thread_info, char* buffer);

#endif 
