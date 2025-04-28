#include "client_handler.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request_parser.h"

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

int process_request(const client_data_t* client_data, Request req) {
    const char* body = "Hello world\n";

    char response[MSG_MAX_SIZE];
    create_response(response, REQUEST_OK, "OK", DEFAULT_CONTENT_TYPE, body,
                    req.keep_alive);

    if (send_response(client_data, response) != 0)
        return 1;

    if (!(req.keep_alive))
        return CLIENT_CLOSED;

    return 0;
}

void* handle_client(void* input_ptr) {
    client_data_t* input = (client_data_t*)input_ptr;
    pthread_t thread_id  = pthread_self();
    char buffer[MSG_MAX_SIZE];

    printf("Client connected, handling in thread %lu\n",
           (unsigned long)thread_id);

    while (true) {
        memset(buffer, 0, MSG_MAX_SIZE);

        ssize_t bytes_read = read(input->client_fd, buffer, MSG_MAX_SIZE);
        if (bytes_read == -1) {
            perror("read() failed");
            continue;
        } else if (bytes_read == 0) {
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_read] = '\0';

        if (input->verbose) {
            printf("Thread %lu received:\n%s", (unsigned long)thread_id,
                   buffer);
            // Print a newline if input did not have one
            if (buffer[bytes_read - 1] != '\n')
                printf("\n");
            printf("===================\n");
        }

        Request req;
        memset(&req, 0, sizeof(req));

        char* err_msg = parse_request(&req, buffer);
        if (err_msg != NULL) {
            send_response(input, err_msg);
            free(err_msg);
            continue;
        }

        if (process_request(input, req) == CLIENT_CLOSED) {
            printf("Client closed connection\n");
            break;
        }
    }

    close(input->client_fd);
    printf("Client FD closed (thread: %lu)\n", (unsigned long)thread_id);
    free(input);
    return NULL;
}
