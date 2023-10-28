#ifndef INTERACTION_H
#define INTERACTION_H


void send_login(struct user* valid_user, int socket);

void listen_command(struct user* valid_user, int socket, char* command);

#endif
