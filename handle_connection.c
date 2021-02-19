#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "service.h"
#include "handle_connection.h"

const char command_template[3] = {'c', 'm', 'x'};

char** format_data(char** response, char *str) {
    unsigned int counter = 0;
    char *temp;
    response[0] = strtok_r(str, "\n", &temp);
    do {
        counter+=1;
    } while ((response[counter] = strtok_r(NULL, "\n", &temp)) != NULL && counter <= 2);

    return response;
}

int terminate_request_with_error(int* client, errors error) {
    send_error(client, error);
    close(*client);
    return -1;
}

int terminate_request(int* client) {
    close(*client);
    return -1;
}

int handle_connection(const int max_bytes, int* client) {
    char* formatted[3];
    char length_buffer[64];
    char* buffer;
    int length, service_key, response_read, bytes_read = 0;

    unsigned char has_length = 0;

    struct timeval tv = {2, 0};
    setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

    while(1) {
        if (has_length == 0) {
            response_read = read(*client, length_buffer, 32);

            // verify commands
            char* is_command = strstr(length_buffer, "cmd");

            if (is_command != NULL) {
                char* command[3];
                format_data(command, length_buffer);

                int command_key = atoi(command[2]);

                if (command_key == 0) {
                    get_services(client, command[1]);
                } else {
                    terminate_request_with_error(client, malformed_header);
                }

                terminate_request(client);
                return 0;
            }

            if (response_read <= 0) {
                return terminate_request_with_error(client, reading_header_error);
            }

            length = atoi(length_buffer);

            if (length <= 0) {
                return terminate_request_with_error(client, reading_header_error);
            }

            has_length = 1;
            response_read = 0;
            buffer = malloc(length + 1);
            continue;
        }

        response_read = read(*client, buffer+bytes_read, max_bytes);

        if (response_read < 0 || response_read > length) {
            return terminate_request_with_error(client, reading_error_message);
        }

        bytes_read+=response_read;

        if (bytes_read > length) {
            return terminate_request_with_error(client, reading_error_message);
        }

        if (response_read == 0 || bytes_read == length) {
            break;
        }
    }

    format_data((char**)&formatted, buffer);

    service_key = atoi(formatted[1]);

    if (service_key < 0) {
        free(buffer);
        return terminate_request_with_error(client, malformed_service_key);
    }

    int response = execute_service(service_key, client, formatted[2]);

    if (response != 0) {
        free(buffer);
        return terminate_request_with_error(client, response);
    }

    close(*client);
    free(buffer);

    return 0;
}