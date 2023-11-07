#ifndef AUTH_H
#define AUTH_H

#include "lib.h"
#include "client-handler.h"

struct user {
    char* username;
    char* password;
    time_t blocked_time;
    int attempt;
    bool isActive;
    char* addr;
    int port; 
    int socket;
    char* last_log_on;
    char** group;
    int num_group; // the number of groups that a user is in
    struct user* next;
};
typedef struct user user;

user* create_node(user* next);

user* load_credentials(int max_attempt);

user* return_user(char *username, struct user* valid_users);

char* password_prompt(int socket);

int password_phase(user* attempt_user, int socket, thread_info* thread_info);

int login(thread_info* thread_info);

void print_all_valided_user(user* lst);

void print_user(user* user);


#endif
