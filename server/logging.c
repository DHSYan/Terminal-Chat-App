#include "logging.h"
#include "auth.h"
#include "lib.h"
#include "const.h"
#include <ctype.h>
#include <string.h>

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
    FILE* userlog = init_logging("userlog.txt");
    time_t timer = time(NULL);
    char* time = malloc(sizeof(char)*SMALL_BUF);
    strcpy(time, asctime(localtime(&timer)));
    int len_time = strlen(time);
    time[len_time-1] = '\0'; // to remove the whitespace at the end
    
    int seq_num = thread_info->global_info->active_user_seq_num;

    thread_info->global_info->active_user_seq_num++;

    char* addr = thread_info->addr;
    int udp_port = thread_info->udp_port;

    fprintf(userlog, "%d; %s; %s; %s; %d\n", seq_num, time, username, addr, udp_port);
    fclose(userlog);
}

void log_msgto(thread_info* thread_info, char* raw_message) {
    char username[SMALL_BUF];
    memset(username, 0, SMALL_BUF);
    strcpy(username, thread_info->thread_user->username);

    FILE* logfile = init_logging("messagelog.txt");

    time_t timer = time(NULL);
    char* time = malloc(sizeof(char)*SMALL_BUF);
    strcpy(time, asctime(localtime(&timer)));
    int len_time = strlen(time);
    time[len_time-1] = '\0'; // to remove the whitespace at the end
   
    int seq_num = thread_info->global_info->message_seq_sum;
    thread_info->global_info->message_seq_sum++;

    fprintf(logfile, "%d; %s; %s; %s\n\n", seq_num, time, username,
            raw_message);
    fclose(logfile);
}

void log_groupchat(thread_info* thread_info, char* groupname,
        char* username,
        int seq_num,
        char* raw_message) {

    char filename[SMALL_BUF];
    memset(filename, 0, SMALL_BUF);
    strcat(filename, groupname);
    strcat(filename, "_messagelog.txt");
    FILE* logfile = init_logging(filename);

    time_t timer = time(NULL);
    char* time = malloc(sizeof(char)*SMALL_BUF);
    strcpy(time, asctime(localtime(&timer)));
    int len_time = strlen(time);
    time[len_time-1] = '\0'; // to remove the whitespace at the end
   
    fprintf(logfile, "%d; %s; %s; %s\n\n", seq_num, time,
            username,
            raw_message);
    fclose(logfile);

}

