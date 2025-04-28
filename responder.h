#ifndef RESPONDER_H
#define RESPONDER_H

#include "client_handler.h"

void create_response(char* final_str, int status_code, const char* status_msg,
                     const char* content_type, const char* body,
                     bool keep_alive);

int send_response(const client_data_t* client_data, const char* response);

#endif
