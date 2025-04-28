#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "client_handler.h"

#define METHOD_SIZE 8
#define PATH_SIZE 256
#define VER_SIZE 16
#define HEADER_MAX_SIZE 64
#define CRLF "\r\n"

typedef struct {
    char method[METHOD_SIZE];
    char path[PATH_SIZE];
    char version[VER_SIZE];
    bool keep_alive;
} Request;

int parse_request(const client_data_t* client_data, Request* req,
                    const char* input_request);

#endif
