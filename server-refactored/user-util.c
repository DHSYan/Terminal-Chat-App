#include "lib.h"
#include "user-util.h"
#include "const.h"

int print_active_user(user* valid_users, user* thread_user) {
    int i = 0;
    char* send_buffer = malloc(sizeof(char)*SMALL_BUF);
    for (user* cur = valid_users; cur; cur=cur->next) {
        if (cur->isActive && cur != thread_user) {
            sprintf(send_buffer, 
                    "[info]|"
                    "Username: %s\n"
                    "    Time since Active: %s\n"
                    "    IP: %s\n"
                    "    Port Num: %d\n", 
                    cur->username, 
                    cur->last_log_on,
                    cur->addr,
                    cur->port);
            i++;
        }
    }

    if (i == 0) {
        send(thread_user->socket, "[info]|no other active user\n", SMALL_BUF, 0);
    } else {
        send(thread_user->socket, send_buffer, SMALL_BUF, 0);
    }
    return 0; // for system caller

}
