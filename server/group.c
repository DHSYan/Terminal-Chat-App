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
        if (strcmp(groupname, cur->group) == 0) {
            return true;
        }
    }
    return false;
}

// Return value:
// 0: succuess
// -1: group already exist
int create_group(struct user* valid_user, char* arguments) {
    char* parsed;
    parsed = strtok(arguments, " ");
    // int i = 0;
    char groupname[50];

    if (isgroupexist(valid_user, groupname)) {
        return -1;
    } else {
        parsed=strtok(NULL, " "); // get rid of "/creategroup"
        strcpy(groupname, parsed); 
        parsed=strtok(NULL, " "); // 

        while (parsed) {
            add_to_group(groupname, return_user(parsed, valid_user));
            // strcpy(return_user(parsed, valid_user)->group, parsed);
            parsed=strtok(NULL, " ");
        }
    }
    return 0;
}


void add_to_group(char* groupname, struct user* user) {
    // user->next_user_in_group = group->head_of_group;
    // group->head_of_group = user;
    strcpy(user->group, groupname);


    // return group;
}
