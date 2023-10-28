#include "auth.h"
#include "stdlib.h"
#include "interaction.h"
#include "util.h"
#define SMALL_BUF 100
#define BUF 1024

void send_login(struct user* valid_user, int socket) {
    print_all_valided_user(valid_user);

    send(socket, "\n[server][input] Enter username: ", SMALL_BUF, 0);
    char* username = malloc(sizeof(char)*BUF);
    recv(socket, username, BUF, 0);
    remove_trailing_space(username);


    int login_username_res = login_username(valid_user, username);
    if (login_username_res == 0) {
        send(socket, "\n[server][input] Enter passward: ", SMALL_BUF, 0);

        char* password = malloc(sizeof(char)*BUF);
        recv(socket, password, BUF, 0);
        remove_trailing_space(password);

        int login_password_res = 
            login_password(valid_user, username, password);

        if (login_password_res == 0) {
            send(socket, "Welcome!", SMALL_BUF, 0);
        } else {
            send(socket, "bruh wrong password!", SMALL_BUF, 0);
        }

    } else if (login_username_res == -2) {
        send(socket, "You are not registered\n", SMALL_BUF, 0);
    } else if (login_username_res == -3) {
        send(socket, "You are still blocked wait\n", SMALL_BUF, 0);
    }
}


// return value:
// 0 sucess!
// -1 they dc-ed
// -2 not a command feedback
// -3 invalid command
int listen_command(struct user* valid_user, int socket, char* command) {
    char* prompt = malloc(sizeof(char)*100);
    strcpy(prompt, "[server] Command: ");
    send(socket, prompt, strlen(prompt)+1, 0);

    char* buffer = malloc(sizeof(char)*100);
    printf(" |  Listening for commands...\n");
    int recv_res = recv(socket, buffer, 100, 0);

    if (recv_res < 0) {
        printf("Listing for command failed to recv\n");
        return -1;
    }

    char* function = malloc(sizeof(char)*SMALL_BUF);
    char* substring = strstr(buffer, "command:");

    // If the command: string don't exist
    // then it is not a command, ignore it
    if (substring == NULL) {
        return -2;
    } else if (strstr(buffer, "login") != NULL) {
        printf("   | command is login, sending it...\n");
        send_login(valid_user, socket);
        return 0;
    } else {
        printf("What is this command? '%s'\n", buffer);
        return -3;
    }
}
