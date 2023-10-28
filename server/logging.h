#ifndef LOGGING_H
#define LOGGING_H

#include "client_handler.h"
#include "stdlib.h"
FILE* init_logging(char* filename);

// Run on successful logins.
void log_login(struct thread_info* thread_info, char* username);
#endif
