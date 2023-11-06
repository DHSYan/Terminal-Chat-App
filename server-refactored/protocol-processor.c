#include "lib.h"

#include "protocol-processor.h"
#include "string-util.h"
#include "auth.h"

// msg should be a heap allocated string
// returns -1 when the client dc-ed, or should be dc-ed
int system_caller(char *msg, thread_info* thread_info) {
    // Prep the message
    remove_trail_whitespace(msg);

    int res = -1;
    if(strcmp(msg, "/login") == 0) {
        printf("login detected\n");
        res = login(thread_info);
    } else {
        printf("This is not a recognized command: %s", msg);
    }

    return res;
}
