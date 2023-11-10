#include "util.h"
#include <string.h>
#include <ctype.h>

void remove_trail_whitespace(char* string) {
    int len = strlen(string);
    for (int i = len-1; isspace(string[i]); i--) {
        string[i] = '\0';
    }
}

