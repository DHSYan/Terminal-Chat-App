#include "lib.h"
#include "user-util.h"
#include "const.h"
#include <string.h>

int print_active_user(user* valid_users, user* thread_user) {
    int i = 0;
    char mid[SMALL_BUF];
    memset(mid, 0, SMALL_BUF);

    char raw[SMALL_BUF];
    memset(raw, 0, SMALL_BUF);

    char* send_buffer = malloc(sizeof(char)*SMALL_BUF);
    memset(send_buffer, 0, SMALL_BUF);
    strcat(send_buffer, "[info]");
    for (user* cur = valid_users; cur; cur=cur->next) {
        if (cur->isActive && cur != thread_user) {
            sprintf(mid, 
                    "|Username: %s\n"
                    "    Time since Active: %s"
                    "    IP: %s\n"
                    "    UDP Port Num: %d\n", 
                    cur->username, 
                    cur->last_log_on,
                    cur->addr,
                    cur->udp_port);
            strcat(send_buffer, mid);
            strcat(send_buffer, "\n");
            strcat(raw, mid);
            strcat(raw, "\n");
            memset(mid, 0, SMALL_BUF);
            i++;
        }
    }

    if (i == 0) {
        send(thread_user->socket, "[info]|no other active user\n", SMALL_BUF, 0);
    } else {
        send(thread_user->socket, send_buffer, SMALL_BUF, 0);
    }
    printf("%s issued /activeuser command\n", thread_user->username);
    printf("\nReturn Messages:\n%s \n", raw);
    return 0; // for system caller

}
