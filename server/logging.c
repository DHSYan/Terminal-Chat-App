#include "stdlib.h"
#include <time.h>

// Functionality needed
// 1. Write to a file
// 2. Generate the file if not alr exist
//

// Deals with: 
// File creation
FILE* init_logging(char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    // fclose(file)
    return file;
}

// Run on successful logins.
void log_login(int seq_num, FILE* file, char* username, char* addr, char* port) {
    // Active User Sequence Number
        // I think think this probabliy be provided by the main.
    time_t timer = time(NULL);
    char* time = malloc(sizeof(char)*SMALL_BUF);
    strcpy(time, asctime(localtime(&timer)));

    fprintf(file, "%d; %s; %s; %s; %s\n", 1, time, username, addr, port);
}
