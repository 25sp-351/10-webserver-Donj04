#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <stdbool.h>

typedef struct {
    int client_fd;
    bool verbose;  // Enables printing more messages to server terminal
} client_data_t;

// Receives and processes the HTTP/1.1 request given by the client, sending back
// a response to the client file descriptor
void* handle_client(void* input_ptr);

#endif
