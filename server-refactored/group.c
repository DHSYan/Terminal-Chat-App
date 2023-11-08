#include "client-handler.h"
#include "lib.h"
#include "const.h"
#include "group.h"
#include "auth.h"
#include "msgto.h"
#include "string-util.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

group* create_group_node(char* groupname, group* next) {
    group* res = malloc(sizeof(struct group));
    res->name = malloc(sizeof(char) * SMALL_BUF);
    remove_trail_whitespace(groupname);
    strcpy(res->name, groupname);
    res->next = next;
    res->joined = false;
    return res;
}

group* return_group(user* valid_users, char* groupname) {
    for (struct user* cur = valid_users; cur; cur=cur->next) {
        for (group* cur_group = cur->grouplst;
             cur_group;
             cur_group=cur_group->next) {
            if (strcmp(cur_group->name, groupname) == 0) {
                return cur_group;
            }
        }
    }
    return NULL;
}

void add_to_group(char* groupname, user* user) {
    user->grouplst = create_group_node(groupname, NULL); 
}

int join_group(char* arg, user* thread_user) {
    
    char* parsed;
    parsed = strtok(arg, " ");
    parsed = strtok(NULL, " ");

    char groupname[SMALL_BUF];
    strcpy(groupname, parsed);

    char* message = malloc(sizeof(char)*SMALL_BUF);
    group* group_target = return_group(thread_user, groupname);
    if (group_target == NULL) {
        sprintf(message, "[info]|Group %s doesn't exist\n", groupname);
    } else {
        sprintf(message, "[info]|Joined the group: %s sucessfully\n", groupname);
        group_target->joined = true;
    }
    send(thread_user->socket, message, SMALL_BUF, 0);

    return 0;
}

int create_group(char* arguments, thread_info* thread_info) {
    user* valid_users = thread_info->global_info->valid_users;
    user* thread_user = thread_info->thread_user;

    char* err = malloc(sizeof(char)*SMALL_BUF);

    char* parsed;
    parsed = strtok(arguments, " ");
    parsed = strtok(NULL, " "); //get rid of "/creategroup"

    if (parsed == NULL) { // groupname entered?
        send(thread_info->socket, 
            "[info]|usage: /creategroup groupname user1 user2 ...\n",
            SMALL_BUF, 
            0);
        return 0;
    }

    char groupname[50];
    strcpy(groupname, parsed);
    parsed = strtok(NULL, " ");

    if (parsed == NULL) {
        send(thread_info->socket, 
                "[info]|Please enter at least one more active users\n", 
                SMALL_BUF,
                0);
        return 0;
    }

    if (return_group(valid_users, groupname) != NULL) {
        sprintf(err, 
                "[info]|A group chat (Name: %s) already exists\n",
                groupname);
        send(thread_info->socket, err, SMALL_BUF, 0);
        return 0;
    } else {

        while (parsed != NULL) {
            user* usertoadd = return_user(parsed, valid_users);
            if (usertoadd == NULL) {
                sprintf(err, "[info]|%s is not an existing user\n", parsed);
                send(thread_info->socket, err, SMALL_BUF, 0);
            } else {
                if (usertoadd->isActive) {
                    add_to_group(groupname, usertoadd);
                } else {
                    sprintf(err,
                            "[info]|User: %s is not active, Not added\n",
                            usertoadd->username);
                    send(thread_info->socket, 
                         err,
                         SMALL_BUF,
                         0);
                }
            }
            parsed=strtok(NULL, " ");
        }

        // The Person who issue the command automatically gets joined
        add_to_group(groupname, thread_user);
        char modified_groupname[SMALL_BUF];
        strcpy(modified_groupname, "/joingroup ");
        strcat(modified_groupname, groupname);
        join_group(modified_groupname, thread_user);
    }

    return 0;
}


int group_msg(char* arguments, thread_info* thread_info) {
    user* valid_users = thread_info->global_info->valid_users;
    user* thread_user = thread_info->thread_user;

    char error_res[SMALL_BUF];

    char* parsed;
    parsed = strtok(arguments, " ");
    parsed = strtok(NULL, " "); //get rid of "/creategroup"

    if (parsed == NULL) { // Entered in Groupname?
        send(thread_info->socket, 
            "[info]|usage: /groupmsg groupname message ...\n",
            SMALL_BUF, 
            0);
        return 0;
    }

    char groupname[50];
    strcpy(groupname, parsed); 
    parsed = strtok(NULL, " "); //getting rid of groupname

    if (parsed == NULL) { // Message Entered?
        send(thread_info->socket, 
            "[info]|Can not send an empty message\n",
            SMALL_BUF, 
            0);
        return 0;
    }


    group* grouptarget = return_group(valid_users, groupname); 
    if (grouptarget == NULL) { // group exist?
        sprintf(error_res, 
                "[info]|A group chat (Name: %s) does NOT exists\n",
                groupname);
        send(thread_info->socket, error_res, SMALL_BUF, 0);
        return 0;

    }

    group* isthreaduseringroup = return_group(thread_user, groupname);
    if (isthreaduseringroup->joined == false) { // is thread_user in group?
        sprintf(error_res, 
                "[info]|Please join the group before sending messages\n");
        send(thread_info->socket, error_res, SMALL_BUF, 0);
        return 0;
    }

    // Building up the message, after strtok-ing it
    // protocol format will be taken care of by better_create_message();
    char final_message[SMALL_BUF]; 
    memset(final_message, 0, SMALL_BUF);
    while (parsed) {
        printf("Strcatting the Message: %s..\n", parsed);
        strcat(final_message, parsed);
        strcat(final_message, " ");
        parsed = strtok(NULL, " ");
    }
   
    for (user* cur = valid_users; cur; cur=cur->next) {
        group* cur_group = return_group(cur, groupname);
        if (cur_group != NULL) { // If (ingroup) 
            if (cur == thread_user) { // is thread_user
                sprintf(error_res, 
                        "[info]|Group chat message sent\n");
                send(thread_user->socket, error_res, SMALL_BUF, 0);
            } else if (cur_group->joined == true) {
                send_message(
                        better_create_message(cur->username,
                                              groupname,
                                              final_message,
                                              thread_info),
                        valid_users);
            } 
        }
    }

    return 0;
}
