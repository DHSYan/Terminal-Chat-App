#ifndef MESSAGING_H
#define MESSAGING_H

#include "stdlib.h"

struct message {
    char username[SMALL_BUF];
    char msg[SMALL_BUF];
    int socket;
};

struct message* create_message(char* string);

struct message* better_create_message(char* username, char* msg);
void send_message(struct message* message, struct user* valid_users);
#endif
