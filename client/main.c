// This is a chat app program coded for the Course COMP3331 at UNSW
// This is the client app
//
// Author: Sean Yan
// zID: z5521807
//
// Acknowledegment
// All Code borrowed or taken from other resources will be comment before the 
// function or block section
//
//


// #include "connect-to-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

#define SMALL_BUF 2048

struct global_info {
    bool status; 
};
typedef struct global_info global_info;

struct server_message {
    char message[SMALL_BUF];
    int socket;
    global_info* global_info;
};

void remove_trail_whitespace(char* string) {
    int len = strlen(string);
    for (int i = len-1; isspace(string[i]); i--) {
        string[i] = '\0';
    }
}

void p2psendfile(char* addr, char* port, char* filename, char* caller_username) {
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int resolve_addr = getaddrinfo(addr, port, &hints, &res);
    if (resolve_addr < 0) { perror("can't get reicever dadr\n"); };

    int sendsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    FILE* file = fopen(filename, "rb");
    int bytesread;

    char buffer[SMALL_BUF];
    memset(buffer, 0, SMALL_BUF);

    char ok_buffer[SMALL_BUF];

    char message[SMALL_BUF];
    strcat(message, caller_username);
    strcat(message, "|");
    strcat(message, filename);

    sendto(sendsocket, message, SMALL_BUF, 0,
            res->ai_addr, res->ai_addrlen);

    // Tutorial Sample Code
    while((bytesread = fread(buffer, 1, SMALL_BUF, file)) > 0) {
        memset(ok_buffer, 0, SMALL_BUF);
        recvfrom(sendsocket, ok_buffer, SMALL_BUF, 0,
                 NULL, NULL);
        if (strstr(ok_buffer, "[OK]") != NULL) {
            sendto(sendsocket, buffer, SMALL_BUF, 0,
                    res->ai_addr, res->ai_addrlen);
        } else {
            printf("instead of [OK] We got: %s\n", ok_buffer);
            printf("Receiver is not responding");
            return; // break out of the function
        }
        memset(buffer, 0, SMALL_BUF);
    }
    fclose(file);

    sendto(sendsocket, "[DONE]\n", SMALL_BUF, 0,
            res->ai_addr, res->ai_addrlen);

}

void* response(void* server_message) {
    struct server_message* message = (struct server_message*) server_message;
    char* send_buffer = malloc(sizeof(char) * SMALL_BUF);
    char* actual_message = strchr(message->message, '|');

    if (actual_message == NULL) {
        message->global_info->status = false;
        return NULL;
    } 
    printf("\n%s", actual_message);
    if (strstr(message->message, "[input]") != NULL) {
        fgets(send_buffer, 99, stdin);
        send(message->socket, send_buffer, SMALL_BUF, 0);
    } else if (strstr(message->message, "[FIN]") != NULL) {
        message->global_info->status = false;
    } else if (strstr(message->message, "[P2P]") != NULL) {
        char* parsed = strtok(actual_message+1, " ");

        char addr[SMALL_BUF];
        strcpy(addr, parsed);
        printf("addr: %s\n", addr);
        parsed = strtok(NULL, " ");
    

        char port[SMALL_BUF];
        strcpy(port, parsed);
        printf("port: %s\n", port);
        parsed = strtok(NULL, " ");

        char filename[SMALL_BUF];
        strcpy(filename, parsed);
        printf("filename: %s\n", filename);
        parsed = strtok(NULL, " ");

        char caller_username[SMALL_BUF];
        strcpy(caller_username, parsed);
        printf("caller_username: %s\n", caller_username);

        p2psendfile(addr, port, filename, caller_username);
    }

    return NULL;
}


void* receivefile(void* socket) {
    int udp_socket_listen = *((int*) socket);
    printf("We are also listening from a UDP socket: %d\n", udp_socket_listen);
    FILE* file = NULL;

    while (true) {
        char buffer[SMALL_BUF];
        memset(buffer, 0, SMALL_BUF);
    
        struct sockaddr_storage presenter;
        socklen_t presenter_len = sizeof(presenter);
    
        // Waiting For "username|file"
        recvfrom(udp_socket_listen, buffer, SMALL_BUF,
                 0, (struct sockaddr*) &presenter, &presenter_len);
        printf("We got from udp: %s\n", buffer);
        printf("\n\nhi\n\n");
    
        char username[SMALL_BUF];
        char filename[SMALL_BUF];
    
        char* parsed = strtok(buffer, "|");
        printf("Username: %s\n", parsed);
        strcpy(username, parsed);
        remove_trail_whitespace(username);
    
        parsed = strtok(NULL, "|");
        printf("filename: %s\n", parsed);
        strcpy(filename, parsed);
    
        char recvfilename[SMALL_BUF];
        strcat(recvfilename, username);
        strcat(recvfilename, "_");
        strcat(recvfilename, filename);
        printf("we are receving the file: %s\n", recvfilename);
    
    
        file = fopen(recvfilename, "wb");
        while(strstr(buffer, "[DONE]") == NULL) {
            memset(buffer, 0, SMALL_BUF);
            sendto(udp_socket_listen,
                   "[OK]\n",
                   SMALL_BUF, 
                   0,
                   ((struct sockaddr*)&presenter), 
                   ((struct sockaddr_in*)&presenter)->sin_len);
    
            memset(buffer, 0, SMALL_BUF);
            recvfrom(udp_socket_listen,
                     buffer,
                     SMALL_BUF,
                     0,
                     (struct sockaddr*) &presenter,
                     &presenter_len);
            if (strstr(buffer, "[DONE]") != NULL) {
                fclose(file);
            } else {
                fwrite(buffer, 1, SMALL_BUF, file);
            }
        }
        fclose(file);
        printf("File Transfer Complete\n");
        printf("\n|Enter Command (/msgto, /activeuser, /creategroup, "
                "/joingroup, /groupmsg, /p2pvideo ,/logout):\n");
    }
    fclose(file);
    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("usage: ./client server_IP server_port client_udp_server_port\n");
        return -1;
    }

    const char* server_addr = argv[1];
    const char* server_port = argv[2];
    const char* my_udp_port = argv[3];

    //////////////////////////////UDP SETUP/////////////////////////////////////
    struct addrinfo udp_hints;
    struct addrinfo* udp_res;

    memset(&udp_hints, 0, sizeof(udp_hints));
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;
    udp_hints.ai_flags = AI_PASSIVE;

    // Beej
    int udp_get_addr_res = getaddrinfo(NULL, my_udp_port, &udp_hints, &udp_res);
    int udp_socket_listen = 
        socket(udp_res->ai_family, 
                udp_res->ai_socktype,
                udp_res->ai_protocol);
    int udp_bind_res = 
        bind(udp_socket_listen, udp_res->ai_addr, udp_res->ai_addrlen);

    pthread_t udp_receive;
    pthread_create(&udp_receive, NULL, receivefile, (void*) &udp_socket_listen);
    
    // printf("\n//////////////////Phase 1, creating the socket and connecting//////////////////\n");
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int get_addr_res = getaddrinfo(server_addr, server_port, &hints, &res);

    int handshake_socket =
        socket(res->ai_family,
                res->ai_socktype,
                res->ai_protocol);
    if (handshake_socket == -1) { printf("socket\n"); }

    int connect_res = connect(handshake_socket, res->ai_addr, res->ai_addrlen);
    if (connect_res < 0) { 
        perror("Couldn't Connect to server\n");
    }

    /////////////////////////////////////////////////////////
    // printf("\n//////////////////Phase 2, sending handshake//////////////////\n");
    
    char* command = malloc(sizeof(char)*SMALL_BUF);
    char* recv_buffer = malloc(sizeof(char)*SMALL_BUF);
    char* handshake = malloc(sizeof(char)*SMALL_BUF);
    char* send_buffer = malloc(sizeof(char)*SMALL_BUF);
    strcpy(handshake, "[client][SYN]|");
    strcat(handshake, my_udp_port);
    strcat(handshake, "\n");

    
    int init_handshack = 
        send(handshake_socket, handshake, strlen(handshake)+1, 0);

    // printf("\n//////////////////Phase 3, wating for SYN ACK//////////////////\n");
    memset(recv_buffer, 0, SMALL_BUF);
    int server_handshake_res = recv(handshake_socket, recv_buffer, SMALL_BUF, 0);

    int allow_to_run = false;
    
    global_info* global_info = malloc(sizeof(struct global_info));
    
    if (strstr(recv_buffer, "[SYNACK]") != NULL) {
        global_info->status = true;
        memset(recv_buffer, 0, SMALL_BUF);
        send(handshake_socket, "[ACK]\n", SMALL_BUF, 0);
        recv(handshake_socket, recv_buffer, SMALL_BUF, 0); // wait for server to send the comm
        send(handshake_socket, "/login\n", SMALL_BUF, 0);
    } else {
        printf("No ACK from Server\n");
        return -1;
    }

    pthread_t response_thread;


    // printf("\n//////////////////Phase 4, Staring the loop//////////////////\n");
    while (global_info->status == true) {
        recv(handshake_socket, recv_buffer, SMALL_BUF, 0);
    
        struct server_message* message = malloc(sizeof(struct server_message));
        message->global_info = global_info;
        strcpy(message->message, recv_buffer);
        memset(recv_buffer, 0, SMALL_BUF);
        message->socket = handshake_socket;
       
        pthread_create(&response_thread, NULL, response, (void*) message);
    }
    return 0;
}

