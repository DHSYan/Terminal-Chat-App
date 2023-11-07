#ifndef GRUOP_H
#define GRUOP_H

#include "auth.h"

struct group {
    char* name;
    bool joined;
    struct group* next;
};
typedef struct group group;


group* create_group_node(char* groupname, group* next);

group* return_group(user* valid_users, char* groupname);

void add_to_group(char* groupname, user* user);

int join_group(char* arg, user* thread_user);

int create_group(char* arguments, thread_info* thread_info);

int group_msg(char* arguments, thread_info* thread_info);
#endif


