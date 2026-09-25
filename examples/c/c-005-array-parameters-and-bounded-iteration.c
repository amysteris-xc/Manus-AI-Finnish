#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_INVALID_VALUE = 2,
    STATUS_SUM_OVERFLOW = 3,
    STATUS_OUTPUT_FAILURE = 4
} Status;

enum {
    MAX_SESSION_MINUTES = 180
};

static Status summarize_session_minutes(const int values[], size_t count,
                                        int *total_out, int *average_out)
{
    int total = 0;

    if (values == NULL || count == 0 || count > (size_t)INT_MAX ||
        total_out == NULL || average_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    for (size_t index = 0; index < count; ++index) {
        int value = values[index];

        if (value < 0 || value > MAX_SESSION_MINUTES) {
            return STATUS_INVALID_VALUE;
        }

        if (total > INT_MAX - value) {
            return STATUS_SUM_OVERFLOW;
        }

        total += value;
    }

    *total_out = total;
    *average_out = total / (int)count;
    return STATUS_OK;
}

static Status write_session_report(const int values[], size_t count,
                                   int total, int average)
{
    if (values == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Session count: %zu\n", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        if (printf("Session %zu: %d minutes\n", index, values[index]) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    if (printf("Total minutes: %d\n", total) < 0 ||
        printf("Average whole minutes: %d\n", average) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

static Status write_error(const char *message)
{
    if (message == NULL || fputs(message, stderr) == EOF) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

int main(void)
{
    const int session_minutes[] = {25, 30, 20, 45, 25};
    const size_t session_count =
        sizeof session_minutes / sizeof session_minutes[0];
    int total_minutes;
    int average_minutes;
    Status status;

    status = summarize_session_minutes(session_minutes, session_count,
                                       &total_minutes, &average_minutes);
    if (status == STATUS_INVALID_VALUE) {
        (void)write_error("Session minutes must be between 0 and 180.\n");
        return EXIT_FAILURE;
    }
    if (status == STATUS_SUM_OVERFLOW) {
        (void)write_error("Session total is too large.\n");
        return EXIT_FAILURE;
    }
    if (status != STATUS_OK) {
        (void)write_error("Cannot summarize the session array.\n");
        return EXIT_FAILURE;
    }

    status = write_session_report(session_minutes, session_count,
                                  total_minutes, average_minutes);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
