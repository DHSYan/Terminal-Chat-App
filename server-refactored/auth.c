#include "group.h"
#include "lib.h"
#include "auth.h"
#include "const.h"
#include "logging.h"
#include "string-util.h"
#include <stdlib.h>
#include <string.h>

user* create_node(user* next) {
    user* res = malloc(sizeof(struct user));
    res->username = malloc(sizeof(char) * SMALL_BUF);
    res->password = malloc(sizeof(char) * SMALL_BUF);
    res->last_log_on = malloc(sizeof(char) *SMALL_BUF);
    res->addr =  malloc(sizeof(char) * SMALL_BUF);
    // res->grouplst = malloc(sizeof(struct group));
    res->grouplst = NULL;
    res->next = next;
    res->isActive = false;
    res->socket = 0;
    res->blocked_time = 0;
    return res;
}

// max_attempt need for preping all the user struct 
// This functions returns a pointer to struct user, that is a 
// linkedlist, and it preps it with the inforamiot needed
user* load_credentials(int max_attempt) {
    // reading a file: https://www.youtube.com/watch?v=fLPqn026DaE
    // struct user* valid_users = malloc(sizeof(struct user));
    struct user* valid_users = create_node(NULL);

    FILE* creds;
    creds = fopen("credentials.txt", "r");
    if (creds == NULL) {
        printf("problem with opening credentials.txt\n");
    } else {
        char c;
        int j = 0;
        int is_user_name_done = false;
        struct user* cur = valid_users;
        while ( (c = fgetc(creds)) != EOF ) {
            if( c == ' ' ) {
                is_user_name_done = true;
                j = 0;
            } else if ( c == '\n' ) {
                // if c == '\n' that means that we have finished
                // scanning this user, now we fill this user's struct
                // with inforamiot that is needed
                cur->next = create_node(NULL);
                cur->attempt = max_attempt;
                cur=cur->next;
                is_user_name_done = false;
                j = 0;
            } else if (!is_user_name_done) {
                cur->username[j] = c;   
                j++;
            } else {
                cur->password[j] = c;
                j++;
            } 
        }
    }
   return valid_users;
}

user* return_user(char *username, struct user* valid_users) { 
    struct user* res;
    for (struct user* cur = valid_users; cur != NULL; cur=cur->next) {
        if(strcmp(cur->username, username) == 0) {
            res = cur;
            return res;
        } 
    }
    return NULL;
}

char* password_prompt(int socket) {
    char* buffer = malloc(sizeof(char) * SMALL_BUF);
    send(socket, "[input]|Enter Password: ", SMALL_BUF, 0);
    recv(socket, buffer, SMALL_BUF, 0);
    return buffer;
}

int password_phase(user* attempt_user, int socket, thread_info* thread_info) {
    time_t seconds;
    time(&seconds);

    if (seconds - attempt_user->blocked_time < 10) { //still blocked
        send(socket, 
                "[info]|You are still blocked, please wait\n",
                SMALL_BUF,
                0);
        return -1;
    } else { // attempt > 0
        while (true) {
            char* buffer = password_prompt(socket);
            remove_trail_whitespace(buffer);

            if (strcmp(buffer, attempt_user->password) == 0) { // correct
                send(socket,
                        "[info]|Welcome to 3331 Chat App\n",
                        SMALL_BUF,
                        0);

                attempt_user->isActive = true;
                strcpy(attempt_user->addr, thread_info->addr);
                attempt_user->port = thread_info->port;

                time_t timer = time(NULL);
                strcpy(attempt_user->last_log_on, asctime(localtime(&timer)));

                attempt_user->socket = socket;

                thread_info->thread_user = attempt_user;

                attempt_user->udp_port = thread_info->udp_port;


                log_login(thread_info, attempt_user->username);

                return 0; // Sucess
            } else {
                attempt_user->attempt--;
                if (attempt_user->attempt == 0) {
                    attempt_user->attempt = 1;
                    time(&seconds);
                    attempt_user->blocked_time = seconds;
                    send(socket,
                            "[info]|You are blocked, please try again later\n",
                            SMALL_BUF,
                            0);
                    return -1;
                }
            }
        }
    }
}

int login(thread_info* thread_info) {
    int socket = thread_info->socket;
    user* valid_users = thread_info->global_info->valid_users;

    char* recv_buffer = malloc(sizeof(char) * SMALL_BUF);
    memset(recv_buffer, 0, SMALL_BUF);

    // Username Prompt Input 
    user* attempt_user = NULL;

    while (attempt_user == NULL) {
        send(socket, "[input]|Enter Username: ", SMALL_BUF, 0);
        recv(socket,  recv_buffer, SMALL_BUF, 0);
        remove_trail_whitespace(recv_buffer);
        attempt_user = return_user(recv_buffer, valid_users);
        if (attempt_user == NULL) {
            send(socket,
                 "[info]|Entered Username is not registered\n",
                 SMALL_BUF,
                 0);

        }    
    }

    return password_phase(attempt_user, socket, thread_info);
}


/////////////////////////////UTILZITY FUNCTIONS/////////////////////////////////
void print_all_valided_user(user* lst) {
    for (user* cur = lst; cur != NULL; cur=cur->next) {
        print_user(cur);
    }
}

void print_user(user* user) {
    if (user == NULL) {
        printf("LOL");
        return;
    }
    printf("-----------------User %s---------\n", user->username);
    printf("    password: %s\n", user->password);
    printf("    Attempt Left: %d\n", user->attempt);
    printf("    socket num: %d\n", user->socket);
    printf("    isActive: %d\n", user->isActive);
    printf("----------------end------------------\n");
}


