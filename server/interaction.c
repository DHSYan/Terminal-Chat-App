#include "auth.h"
#include "client_handler.h"
#include "logging.h"
#include "messaging.h"
#include "stdlib.h"
#include "interaction.h"
#include "util.h"
#define SMALL_BUF 100
#define BUF 1024

void send_login(struct thread_info* thread_info) {
    struct user* valid_user = thread_info->global_info->valid_user;
    int socket = thread_info->socket;

    int seq_num = thread_info->global_info->seq_num;
    FILE* userlog = thread_info->global_info->userlog;
    char* addr = thread_info->addr;
    int port = thread_info->port;

    print_all_valided_user(valid_user);

    send(socket, "[server][input]|Enter username: ", SMALL_BUF, 0);
    char* username = malloc(sizeof(char)*BUF);
    recv(socket, username, BUF, 0);
    remove_trailing_space(username);


    int login_username_res = login_username(valid_user, username);
    if (login_username_res == 0) {
        send(socket, "[server][input]|Enter passward: ", SMALL_BUF, 0);

        char* password = malloc(sizeof(char)*BUF);
        recv(socket, password, BUF, 0);
        remove_trailing_space(password);

        int login_password_res = 
            login_password(valid_user, username, password);

        if (login_password_res == 0) {
            send(socket, "[server][message]|Welcome!", SMALL_BUF, 0);
            log_login(thread_info, username);
            struct user* user = 
                return_user(username, valid_user);
            if (user == NULL) {
                printf("No user with %s found\n", username);
            }
            user->socket = socket;
            thread_info->global_info->seq_num++;
        } else {
            send(socket, "[server][message]|bruh wrong password!", SMALL_BUF, 0);
        }

    } else if (login_username_res == -2) {
        send(socket, "[server][message]|You are not registered\n", SMALL_BUF, 0);
    } else if (login_username_res == -3) {
        send(socket, "[server][message]|You are still blocked wait\n", SMALL_BUF, 0);
    }
    printf("finshed with sending_logging\n");
}

// return value:
// 0 sucess!
// -1 they dc-ed
// -2 not a command feedback
// -3 invalid command
int listen_command(struct thread_info* thread_info,
                  int socket, char* command) 
{
    char* prompt = malloc(sizeof(char)*100);
    strcpy(prompt, "[server][prompt][input]|Enter Command: ");
    send(socket, prompt, strlen(prompt)+1, 0);

    char* buffer = malloc(sizeof(char)*100);
    printf(" |  Listening for commands...\n");
    int recv_res = recv(socket, buffer, 100, 0);

    if (recv_res < 0) {
        printf("Listing for command failed to recv\n");
        return -1;
    }

    char* function = malloc(sizeof(char)*SMALL_BUF);
    char* substring = strchr(buffer, '/');

    // If the command: string don't exist
    // then it is not a command, ignore it
    if (substring == NULL) {
        return -2;
    } else if (strstr(buffer, "/login") != NULL) {
        printf("   | command is login, sending it...\n");
        send_login(thread_info);
        return 0;
    } else if (strstr(buffer, "/msgto") != NULL) {
        printf("   | command is msgto, sending it...\n");
        struct message* message = create_message(buffer);
        printf(" the message is %s\n", message->msg);
        send_message(message, thread_info->global_info->valid_user);
        return 0;
    } else {
        printf("What is this command? '%s'\n", buffer);
        return -3;
    }
}
