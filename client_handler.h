#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <stdbool.h>

typedef struct {
    int client_fd;
    bool verbose;
} client_data_t;

void* handle_client(void* input_ptr);

#endif
