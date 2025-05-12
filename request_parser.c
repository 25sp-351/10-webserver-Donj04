#include "request_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_handler.h"
#include "constants.h"
#include "responder.h"

// Print debug information about client request
void req_print(const Request req) {
    printf(
        "Parsed request data:\n"
        "method: %s\n"
        "path: %s\n"
        "ver: %s\n"
        "connection: %s\n",
        req.method, req.path, req.version,
        (req.keep_alive) ? KEEP_CONNECTION : CLOSE_CONNECTION);
}

// Get the value of the "Connection" header in the request.
// Returns false if the value is "close", true otherwise.
// Invalid values are ignored and treated as the default. ("keep-alive")
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

int parse_request(const client_data_t* client_data, Request* req,
                  const char* input_request) {

    // If request could not be parsed
    if (sscanf(input_request, "%s %s %s", req->method, req->path,
               req->version) != 3)
        send_bad_req_response(client_data, *req, NULL);

    // If HTTP version is unsupported or unknown
    else if (strncmp(req->version, VERSION, MSG_MAX_SIZE) != 0)
        send_bad_ver_response(client_data, *req, NULL);

    // If method is not allowed or unknown
    else if (strncmp(req->method, GET_METHOD, MSG_MAX_SIZE) != 0)
        send_bad_method_response(client_data, *req, NULL);

    // Parsing was successful
    else {
        req->keep_alive = get_connection_header_val(input_request);
        if (client_data->verbose)
            req_print(*req);
        return 0;
    }
    if (client_data->verbose)
        req_print(*req);
    return 1;
}
