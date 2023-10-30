#ifndef MESSAGING_H
#define MESSAGING_H

#include "stdlib.h"

struct message {
    char username[SMALL_BUF];
    char msg[SMALL_BUF];
    int socket;
};

struct message* create_message(char* string);

void send_message(struct message* message, struct user* valid_users);
#endif
