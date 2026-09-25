#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_OUTPUT_FAILURE = 2
} Status;

enum {
    MIN_SESSIONS = 1,
    MAX_SESSIONS = 100,
    MIN_MINUTES_PER_SESSION = 1,
    MAX_MINUTES_PER_SESSION = 180
};

static Status parse_bounded_int(const char *text, int minimum, int maximum,
                                int *result)
{
    char *end = NULL;
    long value;

    if (text == NULL || result == NULL || minimum > maximum) {
        return STATUS_INVALID_ARGUMENT;
    }

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE) {
        return STATUS_INVALID_ARGUMENT;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value < minimum || value > maximum) {
        return STATUS_INVALID_ARGUMENT;
    }

    *result = (int)value;
    return STATUS_OK;
}

static Status calculate_total_minutes(int sessions, int minutes_per_session,
                                      int *result)
{
    if (result == NULL || sessions < MIN_SESSIONS ||
        minutes_per_session < MIN_MINUTES_PER_SESSION) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (sessions > INT_MAX / minutes_per_session) {
        return STATUS_INVALID_ARGUMENT;
    }

    *result = sessions * minutes_per_session;
    return STATUS_OK;
}

static Status write_plan(FILE *stream, int sessions, int minutes_per_session,
                         int total_minutes)
{
    if (stream == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (fprintf(stream, "Sessions: %d\n", sessions) < 0 ||
        fprintf(stream, "Minutes per session: %d\n", minutes_per_session) < 0 ||
        fprintf(stream, "Total planned minutes: %d\n", total_minutes) < 0) {
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

int main(int argc, char *argv[])
{
    int sessions;
    int minutes_per_session;
    int total_minutes;
    Status status;

    if (argc != 3) {
        (void)write_error("Usage: c-003-functions-and-error-returns SESSIONS MINUTES_PER_SESSION\n");
        return EXIT_FAILURE;
    }

    status = parse_bounded_int(argv[1], MIN_SESSIONS, MAX_SESSIONS, &sessions);
    if (status != STATUS_OK) {
        (void)write_error("Invalid session count. Use 1 through 100.\n");
        return EXIT_FAILURE;
    }

    status = parse_bounded_int(argv[2], MIN_MINUTES_PER_SESSION,
                               MAX_MINUTES_PER_SESSION, &minutes_per_session);
    if (status != STATUS_OK) {
        (void)write_error("Invalid minutes per session. Use 1 through 180.\n");
        return EXIT_FAILURE;
    }

    status = calculate_total_minutes(sessions, minutes_per_session,
                                     &total_minutes);
    if (status != STATUS_OK) {
        (void)write_error("Cannot calculate total planned minutes.\n");
        return EXIT_FAILURE;
    }

    status = write_plan(stdout, sessions, minutes_per_session, total_minutes);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
