#include "process_request.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client_handler.h"
#include "constants.h"
#include "request_parser.h"
#include "responder.h"

int process_request(const client_data_t* client_data, const Request req) {
    if (process_calc(client_data, req) == 0)
        return 0;

    if (process_static(client_data, req) == 0)
        return 0;

    return PATH_INVALID;
}

int process_static(const client_data_t* client_data, const Request req) {
    char file_path[FILE_PATH_SIZE];
    getcwd(file_path, FILE_PATH_SIZE);
    strncat(file_path, req.path, FILE_PATH_SIZE);

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return PATH_INVALID;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* byte_list   = malloc(file_size + 1);
    size_t bytes_read = fread(byte_list, 1, file_size, file);

    if (bytes_read != file_size) {
        fprintf(stderr, "Error reading file: %s\n", file_path);
        free(byte_list);
        fclose(file);
        return PATH_INVALID;
    }

    char body[MSG_MAX_SIZE];
    size_t body_len = 0;

    for (size_t ix = 0; ix < file_size; ix++) {
        int written = snprintf(body + body_len, MSG_MAX_SIZE - body_len,
                               "%02x ", (unsigned char)byte_list[ix]);
        body_len += written;
        if (written < 0) {
            fclose(file);
            free(byte_list);
            return PATH_INVALID;
        }
        if (body_len >= MSG_MAX_SIZE) {
            printf("Reached max message length\n");
            break;
        }
    }

    send_ok_response(client_data, req, body);

    fclose(file);
    free(byte_list);
    return 0;
}

int process_calc(const client_data_t* client_data, const Request req) {
    char operator[OP_SIZE];
    int num1;
    int num2;
    int result;

    if (sscanf(req.path, "/calc/%s/%d/%d", operator, & num1, &num2) != 3)
        return PATH_INVALID;

    if (strncmp(operator, "add", OP_SIZE) == 0)
        result = num1 + num2;

    else if (strncmp(operator, "sub", OP_SIZE) == 0)
        result = num1 - num2;

    else if (strncmp(operator, "add", OP_SIZE) == 0)
        result = num1 * num2;

    else if (strncmp(operator, "div", OP_SIZE) == 0)
        result = num1 / num2;

    else
        return PATH_INVALID;

    char result_str[MSG_MAX_SIZE];
    snprintf(result_str, MSG_MAX_SIZE, "%d", result);

    return send_ok_response(client_data, req, result_str);
}
