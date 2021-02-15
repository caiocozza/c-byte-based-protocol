#ifndef HASH_ERRORS_H
#define HASH_ERRORS_H

typedef enum {
    reading_header_error = 1,
    malformed_header,
    malformed_service_key,
    service_not_found,
    malformed_length,
    invalid_length,
    failed_ack,
    reading_error_message
} errors;

static char* error_messages[8] = {
        "Could not read the header sent by the client.",
        "Header received is not in a valid format.",
        "Service key received is not in a valid format.",
        "Service not found.",
        "Message length is not in a valid format.",
        "Message length should be an integer greater than zero.",
        "Could not send the ack to the client.",
        "Error reading the message sent by the client."
};

void send_error(int*, errors);

#endif //HASH_ERRORS_H
