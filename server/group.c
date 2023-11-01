#include "auth.h"
#include "util.h"
#include "stdlib.h"
#include "group.h"
#include <string.h>

// https://en.cppreference.com/w/c/string/byte/strtok
// struct group* create_group(struct group* prev, char *arguments, struct user* valid_users) {
//     char* parsed;
//     parsed = strtok(arguments, " ");
//
//     struct group* group = malloc(sizeof(struct group));
//     group->head_of_group = NULL;
//     group->next_group = prev;
//
//     int i = 0;
//     while(parsed) {
//         if (i == 0) {
//             strcpy(group->name, parsed);
//             i++;
//         } else {
//             add_to_group(group, return_user(parsed, valid_users));
//         }
//         parsed = strtok(NULL, " ");
//     }
//     return group;
// }

bool isgroupexist(struct user* valid_user, char* groupname) {
    for (struct user* cur = valid_user; cur; cur=cur->next) {
        // if (strcmp(groupname, cur->group) == 0) {
        //     return true;
        // }
        for (int i = 0; i < cur->num_group; i++) {
            if (strcmp(groupname, cur->group[i]) == 0) {
                return true;
            }
        }
    }
    return false;
}

// Return value:
// 0: succuess
// -1: group already exist
int create_group(struct user* valid_user, char* arguments) {
    printf("Before parse: %s\n", arguments);
    char* parsed;
    parsed = strtok(arguments, " ");
    parsed = strtok(NULL, " "); //get rid of "/creategroup"
    printf("After 1 parse: %s\n", parsed); // This should show groupname + user

    // int i = 0;
    char groupname[50];
    strcpy(groupname, parsed);
    printf("groupname is: %s\n", groupname);
    parsed = strtok(NULL, " ");
    printf("After 2 parse: %s\n", parsed); // This should show users
        
    //DEGUG
    // if (isgroupexist(valid_user, groupname)) {
    //     printf("The group aready exists\n");
    // } else {
    //     printf("Looks good the group is unique\n");
    // }
    //
    // while (parsed) {
    //     printf("Adding the user: %s\n", parsed);
    //     int len = strlen(parsed);
    //     // for (int i = 0; i < len+1; i++) {
    //     //     if (parsed[i] == '\0') {
    //     //         printf("\\0");
    //     //     } else {
    //     //         printf("%c", parsed[i]);
    //     //     }
    //     // }
    //     // struct user* user = return_user(parsed, valid_user);
    //     // print_user(user);
    //     // add_to_group(groupname, t user *user)
    //     parsed = strtok(NULL, " ");
    // }

    if (isgroupexist(valid_user, groupname)) {
        return -1;
    } else {
        parsed=strtok(NULL, " "); // get rid of "/creategroup"
        strcpy(groupname, parsed); 
        parsed=strtok(NULL, " "); // 
    
       
        while (parsed) {
            printf("Adding the user: %s\n", parsed);
            struct user* usertoadd = return_user(parsed, valid_user);
            if (usertoadd == NULL) {
                printf("return_user() failed\n");
            } else {
                print_user(usertoadd);
                add_to_group(groupname, usertoadd);
                // strcpy(return_user(parsed, valid_user)->group, parsed);
                parsed=strtok(NULL, " ");
            }
        }
    }
    return 0;
}


void add_to_group(char* groupname, struct user* user) {
    // user->next_user_in_group = group->head_of_group;
    // group->head_of_group = user;
    strcpy(user->group[user->num_group], groupname);
    user->num_group++;

    // return group;
}

// int groupmessage(char* arguments, struct user* valid_users) {
//     printf("Before parse: %s\n", arguments);
//     char* parsed; 
//     parsed = strtok(arguments, " ");
//     parsed = strtok(NULL, " "); // getting rid of the /groupmsg
//     printf("After 1 parse: %s\n", parsed);
//
//     char groupname[50];
//
//     strcpy(groupname, parsed);
//     printf("The groupname is %s\n", groupname);
//
//     parsed = strtok(NULL, " "); // getting rid of the groupname
//
//     printf("After 2 parse: %s\n", parsed);
//    
//     for (struct user* cur = valid_users; cur; cur=cur->next) {
//         for (int i = 0; i < cur->num_group; i++) {
//             if (strcmp(cur->group[i], groupname) == 0) {
//                 send(cur->socket, parsed, strlen(parsed), 0);
//             }
//         }
//     }
//     return 0; // nth special
//     //log the messege
//    
// }
