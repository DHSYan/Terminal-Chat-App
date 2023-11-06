#include "lib.h"

#include "protocol-processor.h"
#include "string-util.h"

// msg should be a heap allocated string
// returns false when the client dc-ed
int system_caller(char *msg) {
    // Prep the message
    remove_trail_whitespace(msg);

    if(strcmp(msg, "/login") == 0) {
        printf("login detected\n");
        //login();
    } else {
        printf("This is not a recognized command: %s", msg);
    }

    return 0;
}
