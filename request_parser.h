#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define REQUEST_OK 200
#define BAD_REQUEST 400
#define INVALID_HTTP_VER 505
#define METHOD_NOT_ALLOWED 405

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

char* parse_request(Request* req, const char* input_request);

#endif
