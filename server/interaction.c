#include "auth.h"
#include "stdlib.h"
#include "interaction.h"
#define SMALL_BUF 100
#define BUF 1024

void send_login(int socket) {
    // This could be redundent
    struct user* valid_user = load_credentials();

    send(socket, "[server][input] Enteruser name: ", SMALL_BUF, 0);
    char* username = malloc(sizeof(char)*BUF);
    recv(socket, username, BUF, 0);

    int login_username_res = login_username(valid_user, 2, username);
    if (login_username_res == 0) {
        send(socket, "[server][input] Enter passward: ", SMALL_BUF, 0);
        char* password = malloc(sizeof(char)*BUF);
        recv(socket, password, BUF, 0);
    } else if (login_username_res == -2) {
        send(socket, "You are not registered\n", SMALL_BUF, 0);
    } else if (login_username_res == -3) {
        send(socket, "You are still blocked wait\n", SMALL_BUF, 0);
    }
    
    
}



void listen_command(int socket, char* command) {
    char* prompt = malloc(sizeof(char)*100);
    strcpy(prompt, "[server] Command: ");
    send(socket, prompt, strlen(prompt)+1, 0);


    char* buffer = malloc(sizeof(char)*100);
    printf(" |  Listening for commands...\n");
    int recv_res = recv(socket, buffer, 100, 0);

    if (recv_res < 0) {
        printf("Listing for command failed to recv\n");
    }

    char* function = malloc(sizeof(char)*SMALL_BUF);
    char* substring = strstr(buffer, "command:");

    // If the command: string don't exist
    // then it is not a command, ignore it
    if (substring == NULL) {
        return;
    } else if (strstr(buffer, "login") != NULL) {
        printf("   | command is login, sending it...\n");
        send_login(socket);
    } else {
        printf("What is this command? '%s'\n", buffer);
    }
}
