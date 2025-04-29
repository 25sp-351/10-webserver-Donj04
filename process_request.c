#include "process_request.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client_handler.h"
#include "constants.h"
#include "request_parser.h"
#include "responder.h"

#define CALC_DIR "/calc"
#define STATIC_DIR "/static"

// Writes the binary data from the given file as a string into 'contents'
int write_from_file(char* contents, size_t max_size, const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return PATH_INVALID;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* byte_list = malloc(file_size + 1);
    if (fread(byte_list, 1, file_size, file) != file_size) {
        fprintf(stderr, "Error reading file: %s\n", file_path);
        fclose(file);
        free(byte_list);
        return PATH_INVALID;
    }

    memset(contents, '\0', max_size);
    size_t body_len = 0;
    for (size_t ix = 0; ix < file_size; ix++) {
        // Append each byte in hexadecimal to 'contents'
        int written = snprintf(contents + body_len, max_size - body_len,
                               "%02x ", (unsigned char)byte_list[ix]);
        // Advance offset by num of bytes written
        body_len += written;
        if (written < 0) {
            fprintf(stderr, "Error writing file bytes\n");
            fclose(file);
            free(byte_list);
            return PATH_INVALID;
        }
        if (body_len >= max_size) {
            fprintf(stderr, "Reached max body length\n");
            body_len = max_size;
            break;
        }
    }

    fclose(file);
    free(byte_list);
    return 0;
}

// Sends an HTTP/1.1 response containing the binary file requested from the
// '/static' directory.
// If the path is invalid or not present, returns an error code indicating an
// invalid path was requested.
int process_static(const client_data_t* client_data, const Request req) {
    char requested_path[FILE_PATH_SIZE];
    memset(requested_path, '\0', FILE_PATH_SIZE);

    if (sscanf(req.path, STATIC_DIR "%s", requested_path) != 1) {
        if (client_data->verbose)
            fprintf(stderr, "%s is not a valid path\n", req.path);
        return PATH_INVALID;
    }
    requested_path[FILE_PATH_SIZE - 1] = '\0';  // Null-terminate string

    // Avoid accessing from above the /static directory
    if (strstr(requested_path, "..") != NULL) {
        fprintf(stderr,
                "Detected request to access from above static directory. "
                "Exiting.\n");
        return PATH_INVALID;
    }

    char file_path[FILE_PATH_SIZE];
    getcwd(file_path, FILE_PATH_SIZE);
    strncat(file_path, STATIC_DIR,
            FILE_PATH_SIZE - strnlen(file_path, FILE_PATH_SIZE));
    strncat(file_path, requested_path,
            FILE_PATH_SIZE - strnlen(file_path, FILE_PATH_SIZE));

    if (client_data->verbose)
        printf("Getting file from path: %s\n", file_path);

    char body[MSG_MAX_SIZE];
    int err_code = write_from_file(body, MSG_MAX_SIZE, file_path);
    if (err_code != 0)
        return err_code;

    send_ok_response(client_data, req, body);
    return 0;
}

// Sends an HTTP/1.1 response containing the result of the given operation on
// the numbers in the '/calc' path. Numbers must be 32-bit integers.
// If failed to parse components, returns an error code indicating an invalid
// path was requested.
int process_calc(const client_data_t* client_data, const Request req) {
    char operator[OP_SIZE];
    memset(operator, '\0', OP_SIZE);
    int num1   = 0;
    int num2   = 0;
    int result = 0;
    int converted =
        sscanf(req.path, CALC_DIR "/%3s/%d/%d", operator, &num1, &num2);

    if (client_data->verbose)
        printf(
            "Parsed calc components:\n"
            "operator: %s\n"
            "num1: %d\n"
            "num2: %d\n",
            operator, num1, num2);

    if (converted != 3)
        return PATH_INVALID;

    if (strncmp(operator, "add", OP_SIZE) == 0)
        result = num1 + num2;

    else if (strncmp(operator, "sub", OP_SIZE) == 0)
        result = num1 - num2;

    else if (strncmp(operator, "mul", OP_SIZE) == 0)
        result = num1 * num2;

    else if (strncmp(operator, "div", OP_SIZE) == 0) {
        if (num2 == 0)
            return PATH_INVALID;
        result = num1 / num2;
    } else
        return PATH_INVALID;

    char result_str[MSG_MAX_SIZE];
    snprintf(result_str, MSG_MAX_SIZE, "%d\n", result);

    return send_ok_response(client_data, req, result_str);
}

int process_request(const client_data_t* client_data, const Request req) {
    if (client_data->verbose)
        printf("Processing /calc...\n");
    if (process_calc(client_data, req) == 0)
        return 0;

    if (client_data->verbose)
        fprintf(stderr, "/calc failed. Processing /static...\n");
    if (process_static(client_data, req) == 0)
        return 0;

    if (client_data->verbose)
        fprintf(stderr, "/static failed\n");
    return PATH_INVALID;
}
