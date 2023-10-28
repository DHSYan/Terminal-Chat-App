#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

struct thread_info {
    struct global_info* global_info;
    int socket;
};

struct global_info {
    struct user* valid_user;
    int seq_num;
};

void* client_handler(void* client_info);

#endif
