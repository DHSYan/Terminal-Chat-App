#include "lib.h"
#include "string-util.h"
#include <string.h>
#include "const.h"

void remove_trail_whitespace(char* string) {
    int len = strlen(string);
    for (int i = len-1; isspace(string[i]); i--) {
        string[i] = '\0';
    }
}


void return_msg(char* string) {
    char output[SMALL_BUF];
    memset(output, 0, SMALL_BUF);
    strcat(output, "\nReturn message:\n");
    strcat(output, string);
    strcat(output, "\n");
    printf("%s", output);
}
