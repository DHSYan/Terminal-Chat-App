#include "stdlib.h"
#include "interaction.h"
#define SMALL_BUF 100

void send_login(int socket) {
    printf("Hello world\n");
    send(socket, "hello world\0", 12, 0);
}

void listen_command(int socket, char* command) {
    char* prompt = malloc(sizeof(char)*100);
    strcpy(prompt, "[server] Command: ");
    send(socket, prompt, strlen(prompt)+1, 0);

    char* buffer = malloc(sizeof(char)*100);
    // memset(buffer, 0, strlen(buffer)+1);

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
        printf("   |command is login, sending it...\n");
        send_login(socket);
    } else {
        printf("What is this command? '%s'\n", buffer);
    }
}
