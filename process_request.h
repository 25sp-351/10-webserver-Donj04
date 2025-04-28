#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include "client_handler.h"
#include "request_parser.h"

int process_request(const client_data_t* client_data, const Request req);

int process_static(const client_data_t* client_data, const Request req);

int process_calc(const client_data_t* client_data, const Request req);

#endif
