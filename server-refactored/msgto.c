#include "lib.h"
#include "logging.h"
#include "msgto.h"
#include "const.h"
#include "auth.h"
#include "string-util.h"
#include <string.h>

// A parser that takes in "/msgto username message_content"
// put in into a struct where other function can use it
struct message* create_message(char* string, thread_info* thread_info) {
    struct message* res = malloc(sizeof(struct message));
    char username[SMALL_BUF];
    char message[SMALL_BUF];
    memset(message, 0, SMALL_BUF);
    memset(username, 0, SMALL_BUF);

    strcat(message, "[message]|");

    time_t timer = time(NULL);
    strcat(message, asctime(localtime(&timer)));
    remove_trail_whitespace(message);

    strcat(message, " ");
    strcat(message, thread_info->thread_user->username);
    strcat(message, ": ");


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
    // message[k] = '\n';
    // message[k+1] = '\0';

    strcat(message, "\n\n");
    strcat(message, "|Enter Command (/msgto, /activeuser, /creategroup, /joingroup, /groupmsg, /p2pvideo ,/logout):\n\0");


    strcpy(res->username, username);
    strcat(res->msg, message);
    
    log_msgto(thread_info, string);

    return res;
}

struct message* better_create_message(
        char* username,
        char* groupname,
        char* msg,
        thread_info* thread_info) {

    struct message* res = malloc(sizeof(struct message));
    memset(res->msg, 0, SMALL_BUF);
    strcpy(res->username, username);

    char message[SMALL_BUF];
    memset(message, 0, SMALL_BUF);

    strcat(message, "[message]|");

    time_t timer = time(NULL);
    strcat(message, asctime(localtime(&timer)));
    remove_trail_whitespace(message);
    strcat(message, ", ");

    strcat(message, groupname);
    strcat(message, ", ");



    strcat(message, thread_info->thread_user->username);
    strcat(message, ": ");

    strcat(message, msg);
    strcat(message, "\n\n");

    strcat(message, "|Enter Command (/msgto, /activeuser, /creategroup, /joingroup, /groupmsg, /p2pvideo ,/logout):\n\0");

    strcpy(res->msg, message);

    return res;
}

void send_message(struct message* message, thread_info* thread_info) {
    user* valid_users = thread_info->global_info->valid_users;

    struct user* receiver = return_user(message->username, valid_users);
    send(receiver->socket, message->msg, SMALL_BUF, 0);
    // printf("We send: %s, to %s on %d\n", message->msg,receiver->username, receiver->socket);
}


int msgto(thread_info* thread_info, char* buffer) {
    struct message* message = create_message(buffer, thread_info);
    send_message(message, thread_info);
    return 0; // for system caller
}
