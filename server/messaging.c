#include "auth.h"
#include "stdlib.h"
#include <ctype.h>

#include "messaging.h"
struct message* create_message(char* string) {
    struct message* res = malloc(sizeof(struct message));
    char* command_init = strchr(string, '/');
    char username[SMALL_BUF];
    char message[SMALL_BUF];
    int j = 0;

    for ( command_init+=5  ; isspace(*command_init); command_init++)  {
        username[j] = *command_init;       
        j++;
    }
    username[j] = '\0';

    command_init++; // increment away from the space

    int k = 0;
    for (; command_init; command_init++) {
        message[k] = *command_init;
        k++;
    }
    message[k] = '\0';

    strcpy(res->username, username);
    strcpy(res->msg, message);
    return res;
}

void send_message(struct message* message, struct user* valid_users) {
    struct user* receiver = return_user(message->username, valid_users);
    send(receiver->socket, message->msg, SMALL_BUF, 0);
}
