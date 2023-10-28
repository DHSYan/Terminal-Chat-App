#include "logging.h"
#include "stdlib.h"
#include "util.h"
#include <time.h>

// Functionality needed
// 1. Write to a file
// 2. Generate the file if not alr exist
//

// Deals with: 
// File creation
FILE* init_logging(char* filename) {
    FILE* file = malloc(sizeof(FILE));
    file = fopen(filename, "r");
    if (file == NULL) {
        file = fopen(filename, "w");
    } else {
        file = fopen(filename, "a");
    }
    // fclose(file)
    return file;
}

// Run on successful logins.
void log_login(struct thread_info* thread_info, char* username) {
    // Active User Sequence Number
        // I think think this probabliy be provided by the main.
    FILE* userlog = init_logging("userlog.txt");
    time_t timer = time(NULL);
    char* time = malloc(sizeof(char)*SMALL_BUF);
    strcpy(time, asctime(localtime(&timer)));
    int len_time = strlen(time);
    time[len_time-1] = '\0'; // to remove the whitespace at the end
                             //
    
    int seq_num = thread_info->global_info->seq_num;
    char* addr = thread_info->addr;
    int port = thread_info->port;

    fprintf(userlog, "%d; %s; %s; %s; %d\n", seq_num, time, username, addr, port);
    fclose(userlog);
}
