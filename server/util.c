#include "util.h"
#include <string.h>
int remove_newline(char *string) {
    int len = strlen(string);
    for (int i = 0; i < len; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
    }
    // nth
    return 0;
}
