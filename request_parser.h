#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "client_handler.h"
#include "constants.h"

typedef struct {
    char method[METHOD_SIZE];
    char path[FILE_PATH_SIZE];
    char version[VER_SIZE];
    bool keep_alive;
} Request;

// Parse the request's method, path, HTTP version, and connection status into a
// Request struct, returning nonzero if a value was invalid.
// The resulting struct is written into 'req'.
int parse_request(const client_data_t* client_data, Request* req,
                  const char* input_request);

#endif
