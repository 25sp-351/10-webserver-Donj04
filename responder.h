#ifndef RESPONDER_H
#define RESPONDER_H

#include "client_handler.h"
#include "request_parser.h"

void create_response(char* final_str, int status_code, const char* status_msg,
                     const char* content_type, const char* body,
                     bool keep_alive);

int send_response(const client_data_t* client_data, const char* response);

int send_ok_response(const client_data_t* client_data, const Request req,
                     const char* body);

int send_bad_req_response(const client_data_t* client_data, const Request req,
                          const char* body);

int send_bad_ver_response(const client_data_t* client_data, const Request req,
                          const char* body);

int send_bad_method_response(const client_data_t* client_data,
                             const Request req, const char* body);

#endif
