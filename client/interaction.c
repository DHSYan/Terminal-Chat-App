#include "interaction.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

void send_command(int socket, char* command) {
    char* header = malloc(sizeof(char)*100);
    memset(header, 0, strlen(header));
    strcpy(header, "command:");

    strcat(header, command);
    printf("the final package looks like '%s'\n", header);

    int send_res = send(socket, header, strlen(header)+1, 0);

    if (send_res < 0) {
        printf("send_command didn't send\n");
    } else {
        printf("We sent '%s'...\n", header);
    }
}
