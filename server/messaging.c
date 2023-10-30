#include "auth.h"
#include "stdlib.h"
#include <ctype.h>

#include "messaging.h"
struct message* create_message(char* string) {
    struct message* res = malloc(sizeof(struct message));
    // char* command_init = strchr(string, '/');
    char username[SMALL_BUF];
    char message[SMALL_BUF];
    int j = 0;

    for (int i = 7; !isspace(string[i]); i++)  {
        username[j] = string[i];       
        j++;
    }
    username[j] = '\0';
    // for (int i = 0; i <= 5; i++) {
    //     if (username[i] == '\0') {
    //         puts("\\0");
    //     } else {
    //         printf("%c", username[i]);
    //     }
    // }


    j++;
    int k = 0;
    for (int i = j+7; string[i]; i++) {
        message[k] = string[i];
        k++;
    }
    message[k] = '\0';

    strcpy(res->username, username);
    strcpy(res->msg, message);
    
    // printf("We parsed the username %s\n", res->username);
    // printf("We parsed the password %s\n", res->msg);

    return res;
}

void send_message(struct message* message, struct user* valid_users) {
    struct user* receiver = return_user(message->username, valid_users);
    print_user(receiver);
    send(receiver->socket, message->msg, SMALL_BUF, 0);
}
