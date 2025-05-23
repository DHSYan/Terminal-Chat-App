#ifndef LOGGING_H
#define LOGGING_H

#include "client-handler.h"
#include "stdlib.h"
FILE* init_logging(char* filename);

// Run on successful logins.
void log_login(struct thread_info* thread_info, char* username);

void log_msgto(thread_info* thread_info, char* message);

void log_groupchat(thread_info* thread_info, char* groupname,
        char* username,
        int seq_num,
        char* raw_message);
#endif
