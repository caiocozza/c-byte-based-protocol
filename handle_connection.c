#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "service.h"
#include "handle_connection.h"

char** read_header(char** response, char *str) {
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
    int int_length;
    int int_service_key;
    int result_message;
    unsigned int read_bytes = 0;

    char* header[3];
    char header_buffer[128];

    if (read(*client, header_buffer, sizeof(header_buffer)) < 1) {
        return terminate_request_with_error(client, reading_header_error);
    }

    read_header((char**)&header, header_buffer);

    if (strlen(header[0]) < 1 || strlen(header[1]) <= 0 || strlen(header[2]) <= 0) {
        return terminate_request_with_error(client, malformed_header);
    }

    //TODO: fast atou instead of atoi
    int_service_key = (int)strtol(header[1], NULL, 10);

    if (int_service_key < 0) {
        return terminate_request_with_error(client, malformed_service_key);
    }

    if (service_exists(int_service_key) < 0) {
        return terminate_request_with_error(client, service_not_found);
    }

    //TODO: fast atou instead of atoi
    int_length = atoi(header[2]);

    if (int_length <= 0) {
        return terminate_request_with_error(client, malformed_length);
    }

    if (write(*client, "A", 1) < 0) {
        return terminate_request(client);
    }

    char* received_data = malloc(int_length + 1);

    struct timeval tv = {2, 0};
    setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

    while(1) {
        unsigned char chunk_message[max_bytes];
        result_message = read(*client, chunk_message, max_bytes);

        if (result_message == 0) {
            received_data[read_bytes] = '\n';
            break;
        }

        if(result_message < 0 || result_message > int_length) {
            free(received_data);
            return terminate_request_with_error(client, reading_error_message);
        }

        read_bytes+=result_message;
        
        if(read_bytes > int_length) {
            free(received_data);
            return terminate_request_with_error(client, reading_error_message);
        }

        memcpy(received_data + (read_bytes - result_message), chunk_message, result_message);

        if (read_bytes == int_length) {
            received_data[read_bytes] = '\n';
            break;
        }
    }

    int response = execute_service(int_service_key, client, received_data);

    if (response != 0) {
        free(received_data);
        return terminate_request_with_error(client, response);
    }

    close(*client);
    free(received_data);

    return 0;
}
