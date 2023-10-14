#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

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

struct user* prompt() {
    struct user* res = malloc(sizeof (struct user));
    res->username = malloc(sizeof(char) * 150);
    res->password = malloc(sizeof(char) * 150);
    // Gotta use the stack memeroy
    char username[100];
    char password[100];
    printf("Please Enter username: ");
    scanf("%99s", username);
    strcpy(res->username, username);
    
    printf("\nPlease enter password for %s: ", username);
    scanf("%99s", password);

    strcpy(res->password, password);

    return res;
    
}

bool is_user_equal(struct user* u_lst, struct user* u2, int len) {
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
            printf("Your are blocked, please wait 10 seconds");
            // Multithread here
            break;
        } else {
            user = prompt();
            if (is_user_equal(valid_users, user, 10) == true) {
                printf("Welcome!\n");
                break;
            } else {
                tries++;
            }
        }

    }
    return;
}

void block(struct user user) {
    return;
}

