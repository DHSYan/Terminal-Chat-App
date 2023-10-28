#ifndef LOGGING_H
#define LOGGING_H

#include "stdlib.h"
FILE* init_logging(char* filename);

// Run on successful logins.
void log_login(int seq_num, FILE* file, char* username, char* addr, int port);
#endif
