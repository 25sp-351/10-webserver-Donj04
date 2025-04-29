#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MSG_MAX_SIZE 1024

// HTTP status codes
#define OK_CODE 200
#define BAD_REQ_CODE 400
#define BAD_VER_CODE 505
#define METHOD_NOT_ALLOWED 405

// HTTP error names
#define OK_MSG "OK"
#define BAD_REQ_MSG "Bad Request"
#define BAD_VER_MSG "HTTP Version Not Supported"
#define BAD_METHOD_MSG "Method Not Allowed"

// String constants
#define VERSION "HTTP/1.1"
#define GET_METHOD "GET"
#define DEFAULT_CONTENT_TYPE "text/plain"
#define KEEP_CONNECTION "keep-alive"
#define CLOSE_CONNECTION "close"

// Max string sizes
#define METHOD_SIZE 4
#define FILE_PATH_SIZE 256
#define VER_SIZE 9
#define HEADER_MAX_SIZE 64
#define OP_SIZE 4  // Size of operator string in /calc

// Error code indicating that the requested path could not be processed
#define PATH_INVALID 1

#define CRLF "\r\n"

#endif
