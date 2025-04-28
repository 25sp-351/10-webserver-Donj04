#include "request_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_handler.h"
#include "responder.h"

// Print debug information about client request
void req_print(const Request req) {
    printf(
        "method: %s\n"
        "path: %s\n"
        "ver: %s\n"
        "connection: %s\n",
        req.method, req.path, req.version,
        (req.keep_alive) ? "keep-alive" : "close");
}

bool get_connection_header_val(const char* request) {
    const char* CONNCT_HEADER = "Connection: ";
    const size_t CONNCT_LEN   = 12;

    char* input_dup           = strdup(request);
    char* rest                = input_dup;
    char* token               = strtok_r(rest, CRLF, &rest);
    bool keep_alive           = true;

    while (token != NULL) {
        char* val;
        if (strncmp(token, CONNCT_HEADER, CONNCT_LEN) == 0) {
            val = token + CONNCT_LEN;
            if (strncmp(val, CLOSE_CONNECTION, HEADER_MAX_SIZE) == 0) {
                keep_alive = false;
                break;
            }
        }
        token = strtok_r(NULL, CRLF, &rest);
    }

    free(input_dup);
    return keep_alive;
}

char* parse_request(Request* req, const char* input_request) {
    char* response = malloc(MSG_MAX_SIZE);

    if (sscanf(input_request, "%s %s %s", req->method, req->path,
               req->version) != 3)
        create_response(response, BAD_REQUEST, "Bad Request", NULL, NULL, true);

    else if (strncmp(req->version, VERSION, MSG_MAX_SIZE) != 0)
        create_response(response, INVALID_HTTP_VER,
                        "HTTP Version Not Supported", NULL, NULL, true);

    else if (strncmp(req->method, GET_METHOD, MSG_MAX_SIZE) != 0)
        create_response(response, METHOD_NOT_ALLOWED, "Method Not Allowed",
                        NULL, NULL, true);

    else {
        req->keep_alive = get_connection_header_val(input_request);
        // req_print(*req);
        free(response);
        return NULL;
    }
    // req_print(*req);
    return response;
}
