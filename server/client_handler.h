#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

struct client_thread_info {
    int socket;
    struct user* valid_user;
};

void* client_handler(void* client_info);

#endif
