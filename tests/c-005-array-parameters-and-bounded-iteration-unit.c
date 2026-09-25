#define main c005_program_main
#include "../examples/c/c-005-array-parameters-and-bounded-iteration.c"
#undef main

static int expect(int condition, const char *message)
{
    if (!condition) {
        (void)fputs(message, stderr);
        (void)fputc('\n', stderr);
        return 0;
    }

    return 1;
}

int main(void)
{
    const int valid[] = {25, 30, 20, 45, 25};
    const int too_large[] = {25, 181};
    const int negative[] = {25, -1};
    int total = -1;
    int average = -1;
    Status status;

    status = summarize_session_minutes(valid,
                                       sizeof valid / sizeof valid[0],
                                       &total, &average);
    if (!expect(status == STATUS_OK, "Valid values should succeed.") ||
        !expect(total == 145, "Valid total should be 145.") ||
        !expect(average == 29, "Valid average should be 29.")) {
        return EXIT_FAILURE;
    }

    status = summarize_session_minutes(NULL, 1, &total, &average);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "Null array pointer should be rejected.")) {
        return EXIT_FAILURE;
    }

    status = summarize_session_minutes(valid, 0, &total, &average);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "Zero element count should be rejected.")) {
        return EXIT_FAILURE;
    }

    total = 123;
    average = 456;
    status = summarize_session_minutes(too_large,
                                       sizeof too_large / sizeof too_large[0],
                                       &total, &average);
    if (!expect(status == STATUS_INVALID_VALUE,
                "An over-limit element should be rejected.") ||
        !expect(total == 123 && average == 456,
                "Failed traversal must not replace output values.")) {
        return EXIT_FAILURE;
    }

    status = summarize_session_minutes(negative,
                                       sizeof negative / sizeof negative[0],
                                       &total, &average);
    if (!expect(status == STATUS_INVALID_VALUE,
                "A negative element should be rejected.")) {
        return EXIT_FAILURE;
    }

    (void)fputs("c-005-array-parameters-and-bounded-iteration-unit: PASS\n",
                stdout);
    return EXIT_SUCCESS;
}
