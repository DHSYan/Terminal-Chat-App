#ifndef INTERACTION_H
#define INTERACTION_H

#include "client_handler.h"

void send_login(struct thread_info* thread_info);

int listen_command(struct thread_info* thread_info,
                  int socket, char* command);

#endif
