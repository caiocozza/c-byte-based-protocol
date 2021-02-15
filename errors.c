#include "errors.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void send_error(int* client, errors error) {
    write(*client, error_messages[error - 1], strlen(error_messages[error - 1])+1);
}