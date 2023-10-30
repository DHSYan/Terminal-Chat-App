#ifndef AUTH_H
#define AUTH_H
#include <time.h>
#include <stdbool.h>

struct user {
    char* username;
    char* password;
    time_t blocked_time;
    int attempt;
    bool isActive;
    // char* addr;
    // int port; 
    int socket;
    struct user* next;
};

struct user* create_node(struct user* next);

// Debug
void print_all_valided_user(struct user* list_all_user);

// load the file into a array of users
// This should return a linked list of user
struct user* load_credentials(int max_attempt);


// Output to STDOUT
// If Username Exist (is_registered_user())
// Output passwd prompt to STDOUT
// if Password exist
// return true
// else if max attempted reached
// block()
// loop back
int login_username(struct user *valid_users, char* username);

int login_password(struct user* valid_user, char* username, char* password);

// Checks if username exists within our linked list
// if exist call return_user()
int is_registered_user(char username[], struct user* valid_users);

int is_password_correct(char password[], struct user* user);

// int is_user_valid(struct user* valid_users, struct user* u2);

// Check inputed user against our array of valided user
// void user_validation(struct user* valid_users, int max_attempt); 

// Modify the blocked_time field in struct user to a timestamp
void block(struct user* user);

// Check if the cur time - blocked_time > 1s
// if yes,
// return false (not blocked)
// if on, 
// return true (yes indeed blocked)
int is_blocked(struct user* user);

time_t blocked_for(struct user* user);

// Returns a pointer to the user in the linkedlist
struct user* return_user(char* username, struct user* valid_users);

// This function will prompt client with login prompts
// and 
int send_auth_prompt(int socket);

#endif
