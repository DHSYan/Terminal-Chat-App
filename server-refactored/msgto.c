#include "lib.h"
#include "msgto.h"
#include "const.h"
#include "auth.h"

// A parser that takes in "/msgto username message_content"
// put in into a struct where other function can use it
struct message* create_message(char* string) {
    struct message* res = malloc(sizeof(struct message));
    char username[SMALL_BUF];
    char message[SMALL_BUF];
    strcat(message, "[server][message]|");

    int j = 0;

    for (int i = 7; !isspace(string[i]); i++)  {
        username[j] = string[i];       
        j++;
    }
    username[j] = '\0';

    j++;
    int k = strlen(message);
    for (int i = j+7; string[i]; i++) {
        message[k] = string[i];
        k++;
    }
    message[k] = '\n';
    message[k+1] = '\0';


    strcpy(res->username, username);
    strcat(res->msg, message);
    
    return res;
}

struct message* better_create_message(char* username, char* msg) {
    struct message* res = malloc(sizeof(struct message));
    strcpy(res->username, username);

    char message[SMALL_BUF];
    strcat(message, "[server][message]|");
    strcat(message, msg);
    strcpy(res->msg, message);

    return res;
}

void send_message(struct message* message, user* valid_users) {
    struct user* receiver = return_user(message->username, valid_users);
    send(receiver->socket, message->msg, SMALL_BUF, 0);
    printf("We send: %s, to %s on %d\n", message->msg,receiver->username, receiver->socket);
}


int msgto(thread_info* thread_info, char* buffer) {
    struct message* message = create_message(buffer);
    send_message(message, thread_info->global_info->valid_users);
    return 0; // for system caller
}
