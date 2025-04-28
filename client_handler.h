#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <stdbool.h>

#define MSG_MAX_SIZE 1024
#define CLIENT_CLOSED 2

#define VERSION "HTTP/1.1"
#define GET_METHOD "GET"
#define DEFAULT_CONTENT_TYPE "text/plain"
#define KEEP_CONNECTION "keep-alive"
#define CLOSE_CONNECTION "close"

typedef struct {
    int client_fd;
    bool verbose;
} client_data_t;

void create_response(char* final_str, int status_code, const char* status_msg,
                     const char* content_type, const char* body,
                     bool keep_alive);

int send_response(const client_data_t* client_data, const char* response);

void* handle_client(void* input_ptr);

#endif
