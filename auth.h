#ifndef AUTH_H
#define AUTH_H

struct user {
    char* username;
    char* password;
};

// load the file into a array of users
struct user* load_credentials();

void prompt();

// Check inputed user against our array of valided user
void user_validation(struct user user, int max_attempt); 

// Multithread? 
void block(struct user user);

void print_all_valided_user(struct user* list_all_user, int len);

#endif
