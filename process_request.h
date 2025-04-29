#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

#include "client_handler.h"
#include "request_parser.h"

// Given the client's requested path, will process either the '/calc' or '/static'
// directory with the path components given.
// If the requested operation fails, returns an error code indicating an invalid
// path was requested.
int process_request(const client_data_t* client_data, const Request req);

#endif
