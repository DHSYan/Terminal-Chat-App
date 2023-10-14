#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void print_all_valided_user(struct user* lst, int len) {
    for (int i = 0; i < len; i++) {
        printf("%s ", lst[i].username);
        printf("%s\n", lst[i].password);
    }
}

struct user* load_credentials() {
    // Putting the file content into the array of users
    struct user* valid_users = malloc(sizeof(struct user) * 10); 

    // reading a file: https://www.youtube.com/watch?v=fLPqn026DaE
    FILE* creds;
    creds = fopen("credentials.txt", "r");
    if (creds == NULL) {
        printf("problem with opening credentials.txt\n");
    } else {
        char c;
        int j = 0;
        int i = 0;
        int is_user_name_done = false;
        struct user* current = valid_users;
        current->username = malloc(sizeof(char) * 100);
        current->password = malloc(sizeof(char) * 100);
        while ( (c = fgetc(creds)) != EOF ) {
            // printf("Current Letter: %c\n", c);
            if( c == ' ' ) {
                is_user_name_done = true;
                j = 0;
            } else if ( c == '\n' ) {
                i++;
                is_user_name_done = false;
                // current = (valid_users+1);
                (current+i)->username = malloc(sizeof(char) * 100);
                (current+i)->password = malloc(sizeof(char) * 100);
                j = 0;
            } else if (!is_user_name_done) {
                (current+i)->username[j] = c;   
                j++;
            } else {
                (current+i)->password[j] = c;
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

struct user* prompt(struct user *valid_users) {
    struct user* res = malloc(sizeof (struct user));
    res->username = malloc(sizeof(char) * 150);
    res->password = malloc(sizeof(char) * 150);
    // Gotta use the stack memeroy
    char username[100];
    char password[100];
    printf("Please Enter username: ");
    scanf("%99s", username);
    strcpy(res->username, username);

    if (is_registered_user(username, valid_users, 10) == false) {
        printf("You are not a regitstered User, try again\n");
    } else {
        printf("\nPlease enter password for %s: ", username);
        scanf("%99s", password);

        strcpy(res->password, password);

        return res;
    }
    return NULL;
}

int is_registered_user(char username[], struct user* valid_users, int len) {
    for (int i = 0; i < len; i++) {
        if(strcmp((valid_users+i)->username, username) == 0) {
            return true;
        } 
    }
    return false;
}

int is_user_equal(struct user* u_lst, struct user* u2, int len) {
    if (u2->blocked_time > 0) {
        time_t seconds;
        time(&seconds);
        if (seconds - u2->blocked_time > 10) {
            return -1; // -1 if the user is still blocked
        } else {
            u2->blocked_time = 0; // Can I put 0 into time_t?
        }
    }

    for (int i = 0; i < len; i++) {
        if (strcmp((u_lst+i)->password, u2->password) == 0
                &&
            strcmp((u_lst+i)->username, u2->username) == 0) {
            return true;
        } 
    }
    return false;
}


void user_validation(struct user* valid_users, int max_attempt) {
    struct user* user = NULL;
    int tries = 0;
    while (true) {
        if (tries == max_attempt) {
            printf("Your are blocked, please wait 10 seconds\n");
            block(user);
            // break;
            tries = 0;
        } else {
            user = prompt(valid_users);
            if (user == NULL) {
                printf("Try again\n");
            } else if (is_user_equal(valid_users, user, 10) == true) {
                printf("Welcome!\n");
                break;
            } else if (is_user_equal(valid_users, user, 10) == -1) {
                printf("You are still blocked please wait!\n");
            } else {
                tries++;
            }
        }

    }
    return;
}

void block(struct user* user) {
    // Inspired by https://www.geeksforgeeks.org/time-function-in-c/
    time_t seconds;
    user->blocked_time = time(&seconds);
    return;
}

