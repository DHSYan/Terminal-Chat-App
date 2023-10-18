#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct user* create_node(struct user* next) {
    struct user* res = malloc(sizeof(struct user));
    res->username = malloc(sizeof(char) * 1000);
    res->password = malloc(sizeof(char) * 1000);
    res->next = next;
    return res;
}
void print_all_valided_user(struct user* lst) {
    for (struct user* cur = lst; cur != NULL; cur=cur->next) {
        printf("%s ", cur->username);
        printf("%s\n", cur->password);
    }
}

struct user* load_credentials() {
    // Putting the file content into the array of users
    // struct user* valid_users = malloc(sizeof(struct user) * 10); 
    // for (struct user* cur = valid_users; cur != NULL; cur=cur->next) {
    //     cur->username = malloc(sizeof(char) * 100);
    //     cur->password = malloc(sizeof(char) * 100);
    // }

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
        // current->username = malloc(sizeof(char) * 100);
        // current->password = malloc(sizeof(char) * 100);
        while ( (c = fgetc(creds)) != EOF ) {
            // printf("Current Letter: %c\n", c);
            if( c == ' ' ) {
                is_user_name_done = true;
                j = 0;
            } else if ( c == '\n' ) {
                cur->next = create_node(NULL);
                cur=cur->next;
                is_user_name_done = false;
            
                // current->username = malloc(sizeof(char) * 100);
                // current->password = malloc(sizeof(char) * 100);
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

int login(struct user *valid_users, int max_attempt) {
    char username[100];
    char password[100];

    // while (true) {
    //     while( attempt_num < max_attempt) {
    //         if (is_registered_user(username, valid_users) == false) {
    //             printf("You are not a regitstered User, try again\n");
    //         } else {
    //             attempted_user = return_user(username, valid_users);
    //             printf("\nPlease enter password for %s: ", username);
    //             scanf("%99s", password);
    //             if(is_password_correct(password, attempted_user) == true) {
    //                 return true;
    //             } else {
    //                 printf("The password is not correct");
    //                 attempt_num++;
    //             }
    //         }
    //     }
    //     printf("You have been blocked for 10 seconds\n");
    // }

    struct user* attempted_user;
    while (true) {
        printf("Please Enter username: ");
        scanf("%99s", username);
        if (is_registered_user(username, valid_users) == false) {
            printf("You are not a regitstered User, try again\n");
            continue;
        } else {
            attempted_user = return_user(username, valid_users);
            break;
        }
    }

    int attempt_num = 0;
    while (attempt_num < max_attempt) {
        printf("\nPlease enter password for %s: ", username);
        scanf("%99s", password);
        if(is_password_correct(password, attempted_user) == true) {
            return true;
        } else {
            printf("The password is not correct");
            attempt_num++;
        }
    }

    // if we reach here meaning that the user entered more then max_attempt so
    // we block
    block(attempted_user);

    return -1;
}

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
