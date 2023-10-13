#ifndef AUTH_H
#define AUTH_H

struct user {
    char* username;
    char* password;
};

void prompt();
void user_validation(struct user user, int max_attempt);
void block(struct user user);

#endif
