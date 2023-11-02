#include "auth.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>

struct user* create_node(struct user* next) {
    struct user* res = malloc(sizeof(struct user));
    res->username = malloc(sizeof(char) * 1000);
    res->password = malloc(sizeof(char) * 1000);
    res->last_log_on = malloc(sizeof(char) *1000);
    res->addr =  malloc(sizeof(char) * 1000);
    res->group = malloc(sizeof(char*) * 1000);
    for (int i = 0; i < 1000; i++) {
        res->group[i] = malloc(sizeof(char) * 1000);
    }
    res->num_group = 0;
    res->next = next;
    res->isActive = false;
    res->socket = 0;
    return res;
}
void print_all_valided_user(user* lst) {
    for (user* cur = lst; cur != NULL; cur=cur->next) {
        print_user(cur);
    }
}

void print_user(user* user) {
    if (user == NULL) {
        return;
    }
    printf("\n\n-----------------User %s---------\n", user->username);
    printf("    password: %s\n", user->password);
    printf("    Attempt Left: %d\n", user->attempt);
    printf("    socket num: %d\n", user->socket);
    printf("    isActive: %d\n", user->isActive);
    printf("    # of group: %d\n", user->num_group);
    printf("    next user addr: %p\n", user->next);
    for (int i = 0; i < user->num_group; i++) {
        printf("    Groups: ");
        printf("%s ", user->group[i]);
    }
    printf("\n----------------end------------------\n\n");
}

// max_attempt need for preping all the user struct 
// This functions returns a pointer to struct user, that is a 
// linkedlist, and it preps it with the inforamiot needed
struct user* load_credentials(int max_attempt) {
    // reading a file: https://www.youtube.com/watch?v=fLPqn026DaE
    // struct user* valid_users = malloc(sizeof(struct user));
    struct user* valid_users = create_node(NULL);

    FILE* creds;
    creds = fopen("credentials.txt", "r");
    if (creds == NULL) {
        printf("problem with opening credentials.txt\n");
    } else {
        char c;
        int j = 0;
        int is_user_name_done = false;
        struct user* cur = valid_users;
        while ( (c = fgetc(creds)) != EOF ) {
            if( c == ' ' ) {
                is_user_name_done = true;
                j = 0;
            } else if ( c == '\n' ) {
                // if c == '\n' that means that we have finished
                // scanning this user, now we fill this user's struct
                // with inforamiot that is needed
                cur->next = create_node(NULL);
                cur->attempt = max_attempt;
                cur=cur->next;
                is_user_name_done = false;
                j = 0;
            } else if (!is_user_name_done) {
                cur->username[j] = c;   
                j++;
            } else {
                cur->password[j] = c;
                j++;
            } 
        }
    }
    // Parse the content, 
        // A line is a user
        // grab until whitespace thats the username
        // then the rest until '\n' is the password
   return valid_users;
}

// max_attempted's origin should only come from the command line
// Return value meaning
// -2: Not Registered User
// 0: Registered_user and let user try password
// -3: still blocked need to wait.
// -1: error
int login_username(struct user *valid_users, char* username) {

    // remove_newline(username); // We are going to assume that 
    // string param are already prep

    struct user* attempted_user;
    // while (true) {
        // const char* username_prompt = "Username: ";
        // send(socket, username_prompt, strlen(username_prompt), 0);
        // recv(socket, username, sizeof(username), 0); 
        // remove_newline(username);

    if (is_registered_user(username, valid_users) == false) {
        printf("You are not a regitstered User, try again\n");
        return -2;
        // continue;
    } else {
        attempted_user = return_user(username, valid_users);
        /*
        if (is_blocked(attempted_user) == true
                && blocked_for(attempted_user) < 10) {
            printf("You are blocked please wait\n");
            return -3;
        }*/

        // if attempt == 0, then the user is blocked
        if (attempted_user->attempt == 0 && blocked_for(attempted_user) < 10) {
            printf("You are blocked please wait\n");
            return -3;
        } else if (attempted_user->attempt == 0) {
            // if after 10 has passed, even if attempt == 0, the user
            // has one more chance. And they are blocked another time
            attempted_user->attempt = 1;
            return 0;
        } else {
            return 0;
        }
    }
    // }
}

// Return values
// 0: correct password
// -2: incorrect and blocked
// -1: incorrect but can still retry 
// 3: nothing
int login_password(struct user* valid_user, char* username, char* password) {
    struct user* user = return_user(username, valid_user);
    printf("the user has: %d attempts\n", user->attempt);
    while(user->attempt > 0) {

        int is_correct_passwd = is_password_correct(password, user);
        if (is_correct_passwd == true) {
            printf("[local][log] correct passwd\n");
            return 0;
        } else if (is_correct_passwd == false && user->attempt == 1) {
            printf("[local][log] incorrect password entered, and no more attempt, blocking\n");
            block(user);
            return -2;
        } else {
            printf("[local][log] incorrect password entered\n");
            user->attempt--;
            return -1;
        }
    }
    return 3;
}

// Password
/*
    int attempt_num = 0;
    while (attempt_num < max_attempt) {
        printf("\nPlease enter password for %s: ", username);
        scanf("%99s", password);
        if(is_password_correct(password, attempted_user) == true) {
            return true;
        } else {
            printf("The password is not correct\n");
            attempt_num++;
        }
    }

    // if we reach here meaning that the user entered more then max_attempt so
    // we block
    block(attempted_user);

    return -1;
}
*/

int is_registered_user(char username[], struct user* valid_users) {
    for (struct user* cur = valid_users; cur != NULL; cur=cur->next) {
        if(strcmp(cur->username, username) == 0) {
            return true;
        } 
    }
    return false;
}

// int is_user_equal(struct user* u_lst, struct user* u2, int len) {
//     if (u2->blocked_time > 0) {
//         time_t seconds;
//         time(&seconds);
//         if (seconds - u2->blocked_time > 10) {
//             return -1; // -1 if the user is still blocked
//         } else {
//             u2->blocked_time = 0; // Can I put 0 into time_t?
//         }
//     }
//
//     for (int i = 0; i < len; i++) {
//         if (strcmp((u_lst+i)->password, u2->password) == 0
//                 &&
//             strcmp((u_lst+i)->username, u2->username) == 0) {
//             return true;
//         } 
//     }
//     return false;
// }
//
int is_password_correct(char *password, struct user *user) {
    if(strcmp(user->password, password) == 0) {
        return true;
    } 
    return false;
}


// void user_validation(struct user* valid_users, int max_attempt) {
//     struct user* user = NULL;
//     int tries = 0;
//     while (true) {
//         if (tries == max_attempt) {
//             printf("Your are blocked, please wait 10 seconds\n");
//             block(user);
//             // break;
//             tries = 0;
//         } else {
//             user = prompt(valid_users);
//             if (user == NULL) {
//                 printf("Try again\n");
//             } else if (is_user_equal(valid_users, user, 10) == true) {
//                 printf("Welcome!\n");
//                 break;
//             } else if (is_user_equal(valid_users, user, 10) == -1) {
//                 printf("You are still blocked please wait!\n");
//             } else {
//                 tries++;
//             }
//         }
//
//     }
//     return;
// }

void block(struct user* user) {
    // Inspired by https://www.geeksforgeeks.org/time-function-in-c/
    time_t seconds;
    user->blocked_time = time(&seconds);
    return;
}

int is_blocked(struct user* user) {
    if (user->blocked_time > 0) {
        return true;
    }
    return false;
}

time_t blocked_for(struct user* user) {
    time_t seconds;
    time(&seconds);
    return seconds - user->blocked_time;
}

struct user* return_user(char *username, struct user* valid_users) { 
    struct user* res;
    for (struct user* cur = valid_users; cur != NULL; cur=cur->next) {
        if(strcmp(cur->username, username) == 0) {
            res = cur;
            return res;
        } 
    }
    return NULL;
}

// int valid_users_len(struct user *valid_users) {
//     for (struct user* cur; cur!=NULL
// }

int send_auth_prompt(int socket) {
    const char* username_prompt = "Username: ";
    const char* password_prompt = "Password: ";
    
    send(socket, username_prompt, strlen(username_prompt), 0);
    // recv(socket, void *, size_t, int)
    send(socket, password_prompt, strlen(password_prompt), 0);

    // Nothing Cur
    return 0;
}
