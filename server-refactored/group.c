#include "client-handler.h"
#include "lib.h"
#include "const.h"
#include "group.h"
#include "auth.h"
#include "msgto.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>


bool isgroupexist(user* valid_users, char* groupname) {
    for (struct user* cur = valid_users; cur; cur=cur->next) {
        for (int i = 0; i < cur->num_group; i++) {
            if (strcmp(groupname, cur->group[i]) == 0) {
                return true;
            }
        }
    }
    return false;
}

void add_to_group(char* groupname, user* user) {
    strcpy(user->group[user->num_group], groupname);
    user->num_group++;
}

int create_group(char* arguments, thread_info* thread_info) {
    user* valid_users = thread_info->global_info->valid_users;

    char* parsed;
    parsed = strtok(arguments, " ");
    parsed = strtok(NULL, " "); //get rid of "/creategroup"

    if (parsed == NULL) {
        send(thread_info->socket, 
            "[info]|usage: /creategroup groupname user1 user2 ...\n",
            SMALL_BUF, 
            0);
        return 0;
    }

    char groupname[50];
    strcpy(groupname, parsed);
    parsed = strtok(NULL, " ");

    if (isgroupexist(valid_users, groupname)) {
        char error_res[SMALL_BUF];
        sprintf(error_res, 
                "[info]|A group chat (Name: %s) already exists",
                groupname);
        send(thread_info->socket, error_res, SMALL_BUF, 0);
        return 0;
    } else {
        if (parsed == NULL) {
            send(thread_info->socket, 
                 "[info]|Please enter at least one more active users", 
                 SMALL_BUF,
                 0);
            return 0;
        }
        while (parsed != NULL) {
            user* usertoadd = return_user(parsed, valid_users);
            if (usertoadd == NULL) {
                printf("return_user() failed\n");
            } else {
                if (usertoadd->isActive) {
                    add_to_group(groupname, usertoadd);
                } else {
                    char error_user[SMALL_BUF];
                    sprintf(error_user,
                            "[info]|User: %s is not active, Not added\n",
                            usertoadd->username);
                    send(thread_info->socket, 
                         error_user,
                         SMALL_BUF,
                         0);
                }
                parsed=strtok(NULL, " ");
            }
        }
    }

    return 0;
}


int group_msg(char* arguments, thread_info* thread_info) {
    user* valid_users = thread_info->global_info->valid_users;

    char* parsed;
    parsed = strtok(arguments, " ");
    parsed = strtok(NULL, " "); //get rid of "/creategroup"
                                //
    if (parsed == NULL) {
        send(thread_info->socket, 
            "[info]|usage: /groupmsg groupname message ...\n",
            SMALL_BUF, 
            0);
        return 0;
    }

    char groupname[50];
    strcpy(groupname, parsed); 
    parsed = strtok(NULL, " "); //getting rid of groupname

    if (isgroupexist(valid_users, groupname) == false) {
        char error_res[SMALL_BUF];
        sprintf(error_res, 
                "[info]|A group chat (Name: %s) does NOT exists",
                groupname);
        send(thread_info->socket, error_res, SMALL_BUF, 0);
        return 0;

    }
                                
    if (parsed == NULL) {
        send(thread_info->socket, 
            "[info]|Can not send an empty message\n",
            SMALL_BUF, 
            0);
        return 0;
    }
    

    char final_message[SMALL_BUF]; 
    while (parsed) {
        printf("Strcatting the Message: %s..\n", parsed);
        strcat(final_message, parsed);
        strcat(final_message, " ");
        parsed = strtok(NULL, " ");
    }
   
    for (user* cur = valid_users; cur; cur=cur->next) {
        for (int i = 0; i < cur->num_group; i++) {
            if (strcmp(cur->group[i], groupname) == 0) {
                send_message(
                        better_create_message(cur->username, final_message),
                        valid_users);
            }
        }
    }
    return 0; 
}
