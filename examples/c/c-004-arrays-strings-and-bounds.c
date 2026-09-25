#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_INPUT_FAILURE = 2,
    STATUS_LINE_TOO_LONG = 3,
    STATUS_EMPTY_LABEL = 4,
    STATUS_INVALID_INDEX = 5,
    STATUS_OUTPUT_FAILURE = 6
} Status;

enum {
    MAX_LABEL_LENGTH = 15,
    LABEL_CAPACITY = MAX_LABEL_LENGTH + 2,
    INDEX_LINE_CAPACITY = 32
};

static int discard_remainder_of_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);

    return character == EOF && ferror(stdin) ? 0 : 1;
}

static Status read_line(char *buffer, size_t capacity)
{
    if (buffer == NULL || capacity < 2 || capacity > INT_MAX) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (fgets(buffer, (int)capacity, stdin) == NULL) {
        return STATUS_INPUT_FAILURE;
    }

    if (strchr(buffer, '\n') == NULL) {
        if (!discard_remainder_of_line()) {
            return STATUS_INPUT_FAILURE;
        }
        return STATUS_LINE_TOO_LONG;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    return STATUS_OK;
}

static Status parse_index(const char *text, size_t limit, size_t *result)
{
    char *end = NULL;
    long value;

    if (text == NULL || result == NULL || limit == 0 ||
        limit > (size_t)LONG_MAX) {
        return STATUS_INVALID_ARGUMENT;
    }

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE || value < 0) {
        return STATUS_INVALID_INDEX;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value >= (long)limit) {
        return STATUS_INVALID_INDEX;
    }

    *result = (size_t)value;
    return STATUS_OK;
}

static Status write_indexed_character(const char *label, size_t length,
                                      size_t index)
{
    if (label == NULL || length == 0 || index >= length) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Label: %s\n", label) < 0 ||
        printf("Stored characters: %zu\n", length) < 0 ||
        printf("Character at index %zu: %c\n", index, label[index]) < 0) {
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
    char label[LABEL_CAPACITY];
    char index_line[INDEX_LINE_CAPACITY];
    size_t label_length;
    size_t index;
    Status status;

    if (fputs("Label (1-15 characters): ", stdout) == EOF) {
        return EXIT_FAILURE;
    }

    status = read_line(label, sizeof label);
    if (status == STATUS_LINE_TOO_LONG) {
        (void)write_error("Label is too long. Use at most 15 characters.\n");
        return EXIT_FAILURE;
    }
    if (status != STATUS_OK) {
        (void)write_error("Could not read the label.\n");
        return EXIT_FAILURE;
    }

    label_length = strlen(label);
    if (label_length == 0) {
        (void)write_error("Label must not be empty.\n");
        return EXIT_FAILURE;
    }

    if (printf("Index (0-%zu): ", label_length - 1) < 0) {
        return EXIT_FAILURE;
    }

    status = read_line(index_line, sizeof index_line);
    if (status != STATUS_OK) {
        (void)write_error("Could not read the index.\n");
        return EXIT_FAILURE;
    }

    status = parse_index(index_line, label_length, &index);
    if (status != STATUS_OK) {
        (void)write_error("Index must select an existing label character.\n");
        return EXIT_FAILURE;
    }

    status = write_indexed_character(label, label_length, index);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
