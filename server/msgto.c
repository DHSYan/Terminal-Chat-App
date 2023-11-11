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

    char err[SMALL_BUF];
    
    char* parsed;
    parsed = strtok(string, " ");
    parsed = strtok(NULL, " ");

    if (parsed == NULL) {
        sprintf(err, "[info]|usage: /msgto username message_content\n");
        send(thread_info->socket, err, SMALL_BUF, 0);
        return NULL;
    }
    char username[SMALL_BUF];
    memset(username, 0, SMALL_BUF);
    strcpy(username, parsed);
    parsed = strtok(NULL, " ");

    if (parsed == NULL) {
        sprintf(err, "[info]|Message_content must not be empty\n");
        send(thread_info->socket, err, SMALL_BUF, 0);
        return NULL;
    }

    char message[SMALL_BUF];
    memset(message, 0, SMALL_BUF);

    strcat(message, "[message]|");

    time_t timer = time(NULL);
    strcat(message, asctime(localtime(&timer)));
    remove_trail_whitespace(message);

    strcat(message, " ");
    strcat(message, thread_info->thread_user->username);
    strcat(message, ": ");


    // int j = 0;
    //
    // for (int i = 7; !isspace(string[i]); i++)  {
    //     username[j] = string[i];       
    //     j++;
    // }
    // username[j] = '\0';
    //
    // j++;
    // int k = strlen(message);
    // for (int i = j+7; string[i]; i++) {
    //     message[k] = string[i];
    //     k++;
    // }
    // message[k] = '\n';
    // message[k+1] = '\0';
    char raw_message[SMALL_BUF];
    memset(raw_message, 0, SMALL_BUF);
    while (parsed) {
        strcat(message, parsed);
        strcat(message, " ");
        strcat(raw_message, parsed);
        strcat(raw_message, " ");
        parsed = strtok(NULL, " ");
    }

    strcat(message, "\n\n");
    strcat(message, "|Enter Command (/msgto, /activeuser, /creategroup, /joingroup, /groupmsg, /p2pvideo ,/logout):\n\0");


    strcpy(res->username, username);
    strcat(res->msg, message);
    strcat(res->raw_msg, raw_message);

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

void send_message(struct message* message, thread_info* thread_info, 
        bool logging) {
    if (message == NULL) {
        return;
    }

    char err[SMALL_BUF];

    user* valid_users = thread_info->global_info->valid_users;

    struct user* receiver = return_user(message->username, valid_users);
    if (receiver == NULL) {
        sprintf(err, 
                "[info]|User %s doesn't exist\n", message->username);
        send(thread_info->thread_user->socket, err, SMALL_BUF, 0);
    } else if (receiver->isActive == false) {
        sprintf(err, 
                "[info]|User %s is not active\n", message->username);
        send(thread_info->thread_user->socket, err, SMALL_BUF, 0);

    } else {
        send(receiver->socket, message->msg, SMALL_BUF, 0);

        time_t timer = time(NULL);
        char* time = malloc(sizeof(char)*SMALL_BUF);
        strcpy(time, asctime(localtime(&timer)));
        int len_time = strlen(time);
        time[len_time-1] = '\0'; // to remove the whitespace at the end
                                 //
        printf("%s message to %s '%s' at %s\n", 
                thread_info->thread_user->username,
                receiver->username,
                message->raw_msg,
                time);

        if (logging == true) {
            log_msgto(thread_info, message->raw_msg);
        }

    }
    // printf("We send: %s, to %s on %d\n", message->msg,receiver->username, receiver->socket);
}


int msgto(thread_info* thread_info, char* buffer) {
    
    struct message* message = create_message(buffer, thread_info);
    send_message(message, thread_info, true);

    return 0; // for system caller
}
