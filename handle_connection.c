#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
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

int handle_connection(const int max_bytes, int* client) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int int_length;
    int int_key;
    int result_message;
    unsigned int read_bytes = 0;

    char* header[3];
    char header_buffer[128];

    char* received_data;

    if (read(*client, header_buffer, sizeof(header_buffer)) < 1) {
        close(*client);
        return -1;
    }

    read_header((char**)&header, header_buffer);

    if (strlen(header[0]) < 1 || strlen(header[1]) <= 0 || strlen(header[2]) <= 0) {
        close(*client);
        return -1;
    }

    //TODO: fast atou instead of atoi
    int_key = (int)strtol(header[1], NULL, 10);
    if (int_key < 0) {
        close(*client);
        return -1;
    }

    //TODO: fast atou instead of atoi
    int_length = atoi(header[2]);

    if (int_length <= 0) {
        close(*client);
        return -1;
    }

    if (write(*client, "A", 1) < 0) {
        close(*client);
        return -1;
    }

    received_data = malloc(int_length + 1);

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
            close(*client);
            free(received_data);
            return -1;
        }

        read_bytes+=result_message;
        
        if(read_bytes > int_length) {
            close(*client);
            free(received_data);
            return -1;
        }

        memcpy(received_data + (read_bytes - result_message), chunk_message, result_message);

        if (read_bytes == int_length) {
            received_data[read_bytes] = '\n';
            break;
        }
    }

    execute_service(int_key, client, received_data);

    close(*client);
    free(received_data);

    gettimeofday(&stop, NULL);
    printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    return 0;
}
