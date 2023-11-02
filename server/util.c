#include "util.h"
#include <ctype.h>
#include <string.h>
#include "stdlib.h"
int remove_newline(char *string) {
    int len = strlen(string);
    for (int i = 0; i < len+1; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
    }
    // nth
    return 0;
}

void remove_trailing_space(char* string) {
    int len = strlen(string);
    for (int i = len-1; i >= 0; i--) {
        if (isspace(string[i])) {
            string[i] = '\0';
        }
    }
}

void print_char_obo(char* string) {
    int len = strlen(string);
    for (int i = 0; i < len+1; i++) {
        if (string[i] == '\0') {
            printf("\\0");
        } else {
            printf("%c ", string[i]);
        }
    
    }
}
