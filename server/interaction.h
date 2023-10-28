#ifndef INTERACTION_H
#define INTERACTION_H


void send_login(int socket);

void listen_command(int socket, char* command);

#endif
