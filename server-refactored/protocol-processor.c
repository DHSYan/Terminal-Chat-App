#include "group.h"
#include "lib.h"

#include "msgto.h"
#include "protocol-processor.h"
#include "string-util.h"
#include "auth.h"
#include "user-util.h"

// msg should be a heap allocated string
// returns -1 when the client dc-ed, or should be dc-ed
int system_caller(char *msg, thread_info* thread_info) {
    // Prep the message
    remove_trail_whitespace(msg);

    int res = -1;
    if(strcmp(msg, "/login") == 0) {
        printf("login detected\n");
        res = login(thread_info);
    } else if (strstr(msg, "/msgto") != NULL) {
        printf("/msgto detected\n");
        res = msgto(thread_info, msg);
    } else if (strstr(msg, "/activeuser") != NULL) {
        res = print_active_user(
                thread_info->global_info->valid_users,
                thread_info->thread_user);
    } else if (strstr(msg, "/creategroup") != NULL) {
        res = create_group(msg, thread_info);
    } else if (strstr(msg, "/groupmsg") != NULL) {
        res = group_msg(msg, thread_info);
    } else {
        printf("This is not a recognized command: %s", msg);
    }

    return res;
}
