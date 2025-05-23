#include "group.h"
#include "lib.h"

#include "msgto.h"
#include "protocol-processor.h"
#include "string-util.h"
#include "auth.h"
#include "user-util.h"
#include "p2p.h"



// msg should be a heap allocated string
// returns -1 when the client dc-ed, or should be dc-ed
int system_caller(char *msg, thread_info* thread_info) {
    // Prep the message
    remove_trail_whitespace(msg);

    int res = -1;
    if(strcmp(msg, "/login") == 0) {
        res = login(thread_info);
    } else if (strstr(msg, "/msgto") != NULL) {
        res = msgto(thread_info, msg);
    } else if (strstr(msg, "/activeuser") != NULL) {
        printf("%s issued /activeuser command\n",
                thread_info->thread_user->username);
        res = print_active_user(
                thread_info->global_info->valid_users,
                thread_info->thread_user);
    } else if (strstr(msg, "/creategroup") != NULL) {
        printf("%s issued the /creategroup command\n",
                thread_info->thread_user->username);
        res = create_group(msg, thread_info);
    } else if (strstr(msg, "/groupmsg") != NULL) {
        printf("%s issued /groupmsg\n", thread_info->thread_user->username);
        res = group_msg(msg, thread_info);
    } else if (strstr(msg, "/joingroup") != NULL) {
        printf("%s issued /joingroup\n", thread_info->thread_user->username);
        res = join_group(msg, thread_info->thread_user);
    } else if (strstr(msg, "/logout") != NULL) {
        printf("%s logout\n", thread_info->thread_user->username);
        res = -1;
    } else if (strstr(msg, "/p2pvideo") != NULL) {
        printf("%s issued /p2pvideo\n", thread_info->thread_user->username);
        res = p2p(msg, thread_info);

    } else {
        send(thread_info->socket, "[info]|Invalid Command\n", SMALL_BUF, 0);
        res = 0;
        printf("This is not a recognized command: %s", msg);
    }

    return res;
}
