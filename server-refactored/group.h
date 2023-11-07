#ifndef GRUOP_H
#define GRUOP_H

#include "auth.h"

bool isgroupexist(user* valid_users, char* groupname);

void add_to_group(char* groupname, user* user);

int create_group(char* arguments, thread_info* thread_info);
#endif


