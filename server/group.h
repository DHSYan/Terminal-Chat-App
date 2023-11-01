#ifndef GROUP_H
#define GROUP_H

#include "auth.h"
#include "util.h"
// #include "auth.h"
struct group {
    char name[50];
    struct user* head_of_group;
    struct group* next_group;
};

bool isgroupexist(struct user* valid_user, char* groupname);

// can use strtok and use space as a delim, 
// and just remember that the first one is group name
// and the rest are the user to add
// and call add_to_group() on all the rest
// struct group* create_group(struct group* prev, char* arguments, struct user* valid_users);
int create_group(struct user* valid_user, char* arguments);

// modify the user->next_in_group to point to 
// the current HEAD inside struct group.
// struct group* add_to_group(struct group* group, struct user* user);
void add_to_group(char* groupname, struct user* user);


int groupmessage(char* arguments, struct user* valid_users);

#endif
