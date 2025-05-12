#ifndef RESPONDER_H
#define RESPONDER_H

#include "client_handler.h"
#include "request_parser.h"

// Builds an HTTP/1.1 response using the given parameters, and writes it to
// 'final_str'
void create_response(char* final_str, int status_code, const char* status_msg,
                     const char* content_type, const char* body,
                     bool keep_alive);

// Writes the given response to the client's file descriptor
int send_response(const client_data_t* client_data, const char* response);

// Builds and sends an HTTP/1.1 response using the status code of an OK request.
// An optional body string can be given.
int send_ok_response(const client_data_t* client_data, const Request req,
                     const char* body);

// Builds and sends an HTTP/1.1 response using the status code of a bad request.
// An optional body string can be given.
int send_bad_req_response(const client_data_t* client_data, const Request req,
                          const char* body);

// Builds and sends an HTTP/1.1 response using the status code of an invalid
// HTTP version. An optional body string can be given.
int send_bad_ver_response(const client_data_t* client_data, const Request req,
                          const char* body);

// Builds and sends an HTTP/1.1 response using the status code of a disallowed
// method. An optional body string can be given.
int send_bad_method_response(const client_data_t* client_data,
                             const Request req, const char* body);

#endif
