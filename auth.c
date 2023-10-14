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

    // https://www.youtube.com/watch?v=fLPqn026DaE
    // reading a file 
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

void prompt() {
    return;
}

void user_validation(struct user user, int max_attempt) {
    return;
}

void block(struct user user) {
    return;
}

