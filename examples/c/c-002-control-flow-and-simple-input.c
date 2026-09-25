#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    INPUT_CAPACITY = 64,
    MIN_LESSONS = 1,
    MAX_LESSONS = 100,
    REVIEW_INTERVAL = 5
};

static int parse_lesson_count(const char *text, int *result)
{
    char *end = NULL;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE) {
        return 0;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value < MIN_LESSONS || value > MAX_LESSONS) {
        return 0;
    }

    *result = (int)value;
    return 1;
}

static int discard_remainder_of_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);

    return character == EOF && ferror(stdin) ? 0 : 1;
}

int main(void)
{
    char input[INPUT_CAPACITY];
    int lesson_count;
    int checkpoint_count = 0;

    if (fputs("How many lessons (1-100)? ", stdout) == EOF) {
        return 1;
    }

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            fputs("Input error.\n", stderr);
        } else {
            fputs("No input received.\n", stderr);
        }
        return 1;
    }

    if (strchr(input, '\n') == NULL) {
        if (!discard_remainder_of_line()) {
            fputs("Input error while discarding a long line.\n", stderr);
        } else {
            fputs("Input is too long.\n", stderr);
        }
        return 1;
    }

    if (!parse_lesson_count(input, &lesson_count)) {
        fputs("Enter one whole number from 1 to 100.\n", stderr);
        return 1;
    }

    if (lesson_count == 1) {
        if (printf("Plan: 1 lesson\n") < 0) {
            return 1;
        }
    } else {
        if (printf("Plan: %d lessons\n", lesson_count) < 0) {
            return 1;
        }
    }

    for (int day = 1; day <= lesson_count; ++day) {
        if (day % REVIEW_INTERVAL == 0) {
            ++checkpoint_count;
        }
    }

    if (printf("Review checkpoints: %d\n", checkpoint_count) < 0) {
        return 1;
    }

    if (lesson_count == 1) {
        if (fputs("Start with one focused session.\n", stdout) == EOF) {
            return 1;
        }
    } else {
        if (fputs("Make the plan in short, repeatable blocks.\n", stdout) == EOF) {
            return 1;
        }
    }

    return 0;
}
