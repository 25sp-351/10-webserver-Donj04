#include "responder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"

void create_response(char* final_str, int status_code, const char* status_msg,
                     const char* content_type, const char* body,
                     bool keep_alive) {

    snprintf(final_str, MSG_MAX_SIZE,
             VERSION
             " %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: %s\r\n\r\n"
             "%s",
             status_code, status_msg,
             (content_type != NULL) ? content_type : DEFAULT_CONTENT_TYPE,
             (body != NULL) ? strnlen(body, MSG_MAX_SIZE) : 0,
             (keep_alive) ? KEEP_CONNECTION : CLOSE_CONNECTION,
             (body != NULL) ? body : "");
}

int send_response(const client_data_t* client_data, const char* response) {
    if (client_data->verbose) {
        printf("Sending response to client:\n%s\n", response);
        printf("===================\n");
    }

    if (write(client_data->client_fd, response,
              strnlen(response, MSG_MAX_SIZE)) == -1) {
        perror("write() failed");
        return 1;
    }
    return 0;
}

int send_ok_response(const client_data_t* client_data, const Request req,
                     const char* body) {
    char response[MSG_MAX_SIZE];
    create_response(response, OK_CODE, OK_MSG, DEFAULT_CONTENT_TYPE, body,
                    req.keep_alive);
    return send_response(client_data, response);
}

int send_bad_req_response(const client_data_t* client_data, const Request req,
                          const char* body) {
    char response[MSG_MAX_SIZE];
    create_response(response, BAD_REQ_CODE, BAD_REQ_MSG, DEFAULT_CONTENT_TYPE,
                    body, req.keep_alive);
    return send_response(client_data, response);
}

int send_bad_ver_response(const client_data_t* client_data, const Request req,
                          const char* body) {
    char response[MSG_MAX_SIZE];
    create_response(response, BAD_VER_CODE, BAD_VER_MSG, DEFAULT_CONTENT_TYPE,
                    body, req.keep_alive);
    return send_response(client_data, response);
}

int send_bad_method_response(const client_data_t* client_data,
                             const Request req, const char* body) {
    char response[MSG_MAX_SIZE];
    create_response(response, METHOD_NOT_ALLOWED, BAD_METHOD_MSG,
                    DEFAULT_CONTENT_TYPE, body, req.keep_alive);
    return send_response(client_data, response);
}
