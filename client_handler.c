#include "client_handler.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request_parser.h"
#include "responder.h"

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
