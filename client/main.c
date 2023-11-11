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
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "interaction.h"
#include "util.h"

#define SMALL_BUF 2048

struct global_info {
  bool status;
  int udp_socket;
};
typedef struct global_info global_info;

struct server_message {
  char message[SMALL_BUF];
  int socket;
  global_info *global_info;
};

struct udp_listening {
  int socket;
  global_info *global_info;
};

void sending(char *addr, char *port, char *filename, char *caller_username,
             int socket) {
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int resolve_addr = getaddrinfo(addr, port, &hints, &res);
  if (resolve_addr < 0) {
    perror("can't get reicever dadr\n");
  };

  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Can't open file when sending\n");
    return;
  }

  int bytesread;
  char buffer[SMALL_BUF];
  char ok_buffer[SMALL_BUF];
  char prep_buffer[SMALL_BUF];

  remove_trail_whitespace(caller_username);
  strcat(prep_buffer, caller_username);
  strcat(prep_buffer, "|");
  strcat(prep_buffer, filename);

  sendto(socket, prep_buffer, SMALL_BUF, 0, res->ai_addr, res->ai_addrlen);

  // Tutorial Sample Code
  while ((bytesread = fread(buffer, 1, SMALL_BUF, file)) > 0) {
    recvfrom(socket, ok_buffer, SMALL_BUF, 0, NULL, NULL);
    if (strcmp(ok_buffer, "[OK]") == 0) {
      sendto(socket, buffer, SMALL_BUF, 0, res->ai_addr, res->ai_addrlen);
    } else {
      printf("Receiver is not responding");
      return; // break out of the function
    }
  }

  sendto(socket, "[DONE]", SMALL_BUF, 0, res->ai_addr, res->ai_addrlen);
}

void *response(void *server_message) {
  struct server_message *message = (struct server_message *)server_message;
  char *send_buffer = malloc(sizeof(char) * SMALL_BUF);
  char *actual_message = strchr(message->message, '|');

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
    // some function that start sending
    // We need to store addr and port and filename
    // "[P2P]|addr port filename caller_username"
    char *parsed = strtok(actual_message + 1, " ");
    printf("Looking at %s\n", parsed);

    char addr[SMALL_BUF];
    strcpy(addr, parsed);
    parsed = strtok(NULL, " ");
    printf("Looking at %s\n", parsed);

    char port[SMALL_BUF];
    strcpy(port, parsed);
    parsed = strtok(NULL, " ");
    printf("Looking at %s\n", parsed);

    char filename[SMALL_BUF];
    strcpy(filename, parsed);
    parsed = strtok(NULL, " ");
    printf("Looking at %s\n", parsed);

    char caller_username[SMALL_BUF];
    strcpy(caller_username, parsed);

    sending(addr, port, filename, caller_username,
            message->global_info->udp_socket);
  }

  return NULL;
}

void *listening(void *listening_info) {
  struct udp_listening *info = (struct udp_listening *)listening_info;

  printf("We are also listening from a UDP Port\n");
  int udp_socket_listen = info->socket;

  while (info->global_info->status) {
    char buffer[SMALL_BUF];
    memset(buffer, 0, SMALL_BUF);

    struct sockaddr_storage presenter;
    socklen_t presenter_len = sizeof(presenter);

    // Waiting For "username|file"
    recvfrom(udp_socket_listen, buffer, SMALL_BUF, 0,
             (struct sockaddr *)&presenter, &presenter_len);

    char username[SMALL_BUF];
    char filename[SMALL_BUF];

    char *parsed = strtok(buffer, "|");
    printf("Username: %s\n", parsed);
    strcpy(username, parsed);

    parsed = strtok(NULL, "|");
    printf("filename: %s\n", parsed);
    strcpy(filename, parsed);

    char recvfilename[SMALL_BUF];
    strcat(recvfilename, username);
    strcat(recvfilename, "_");
    strcat(recvfilename, filename);
    printf("We are going to create %s\n", recvfilename);

    FILE *file = fopen(recvfilename, "wb");
    memset(buffer, 0, SMALL_BUF);
    while (strcmp(buffer, "[DONE]") != 0) {
      sendto(udp_socket_listen, "[OK]\n", SMALL_BUF, 0,
             ((struct sockaddr *)&presenter),
             ((struct sockaddr_in *)&presenter)->sin_len);

      recvfrom(udp_socket_listen, buffer, SMALL_BUF, 0,
               (struct sockaddr *)&presenter, &presenter_len);

      fwrite(buffer, 1, SMALL_BUF, file);
    }
    fclose(file);
    printf("File Transfer Complete\n");
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    perror("usage: ./client server_IP server_port client_udp_server_port\n");
    return -1;
  }

  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo udp_hints;
  struct addrinfo *udp_res;

  memset(&udp_hints, 0, sizeof(udp_hints));
  udp_hints.ai_family = AF_INET;
  udp_hints.ai_socktype = SOCK_DGRAM;
  udp_hints.ai_flags = AI_PASSIVE;

  // const char* server_addr = "127.0.0.1";
  const char *server_addr = argv[1];
  const char *server_port = argv[2];
  const char *my_udp_port = argv[3];

  // printf("\n//////////////////Phase 1, creating the socket and
  // connecting//////////////////\n");

  int get_addr_res = getaddrinfo(server_addr, server_port, &hints, &res);
  int handshake_socket =
      socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (handshake_socket == -1) {
    printf("socket\n");
  }
  int connect_res = connect(handshake_socket, res->ai_addr, res->ai_addrlen);
  if (connect_res < 0) {
    perror("Couldn't Connect to server\n");
  }

  // Beej
  // UDP
  int udp_get_addr_res = getaddrinfo(NULL, my_udp_port, &udp_hints, &udp_res);
  int udp_socket_listen =
      socket(udp_res->ai_family, udp_res->ai_socktype, udp_res->ai_protocol);
  int udp_bind_res =
      bind(udp_socket_listen, udp_res->ai_addr, udp_res->ai_addrlen);

  /////////////////////////////////////////////////////////
  // printf("\n//////////////////Phase 2, sending
  // handshake//////////////////\n");

  char *command = malloc(sizeof(char) * SMALL_BUF);
  char *recv_buffer = malloc(sizeof(char) * SMALL_BUF);
  char *handshake = malloc(sizeof(char) * SMALL_BUF);
  char *send_buffer = malloc(sizeof(char) * SMALL_BUF);
  strcpy(handshake, "[client][SYN]|");
  strcat(handshake, my_udp_port); // Send the UDP POrt that the client
                                  // is listeing to to the server
                                  // as part of the SYN Package

  int init_handshack =
      send(handshake_socket, handshake, strlen(handshake) + 1, 0);

  // printf("\n//////////////////Phase 3, wating for SYN
  // ACK//////////////////\n");
  memset(recv_buffer, 0, SMALL_BUF);
  int server_handshake_res = recv(handshake_socket, recv_buffer, SMALL_BUF, 0);

  int allow_to_run = false;

  global_info *global_info = malloc(sizeof(struct global_info));
  global_info->udp_socket = udp_socket_listen;

  if (strstr(recv_buffer, "[SYNACK]") != NULL) {
    global_info->status = true;
    memset(recv_buffer, 0, SMALL_BUF);
    send(handshake_socket, "[ACK]\n", SMALL_BUF, 0);
    recv(handshake_socket, recv_buffer, SMALL_BUF,
         0); // wait for server to send the comm
    send(handshake_socket, "/login\n", SMALL_BUF, 0);
  } else {
    printf("No ACK from Server\n");
    return -1;
  }

  pthread_t response_thread;

  struct udp_listening *udp_info = malloc(sizeof(struct udp_listening));
  udp_info->socket = udp_socket_listen;
  udp_info->global_info = global_info;
  pthread_t udp_listeing_thread;
  pthread_create(&udp_listeing_thread, NULL, listening, (void *)udp_info);

  // printf("\n//////////////////Phase 4, Staring the
  // loop//////////////////\n");
  while (global_info->status == true) {
    recv(handshake_socket, recv_buffer, SMALL_BUF, 0);

    struct server_message *message = malloc(sizeof(struct server_message));
    message->global_info = global_info;
    strcpy(message->message, recv_buffer);
    memset(recv_buffer, 0, SMALL_BUF);
    message->socket = handshake_socket;

    pthread_create(&response_thread, NULL, response, (void *)message);
  }
  return 0;
}
