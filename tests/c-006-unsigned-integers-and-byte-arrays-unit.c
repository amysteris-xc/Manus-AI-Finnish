#define main c006_program_main
#include "../examples/c/c-006-unsigned-integers-and-byte-arrays.c"
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
    const uint8_t mixed[] = {
        UINT8_C(0x00), UINT8_C(0xFF), UINT8_C(0x00), UINT8_C(0x7F)
    };
    size_t zero_count = 99;
    Status status;

    status = count_zero_bytes(mixed, sizeof mixed / sizeof mixed[0],
                              &zero_count);
    if (!expect(status == STATUS_OK, "A valid byte buffer should succeed.") ||
        !expect(zero_count == 2, "The buffer should contain two zero bytes.")) {
        return EXIT_FAILURE;
    }

    status = count_zero_bytes(NULL, 1, &zero_count);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "A null byte buffer should be rejected.")) {
        return EXIT_FAILURE;
    }

    status = count_zero_bytes(mixed, 0, &zero_count);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "A zero length should be rejected by this interface.")) {
        return EXIT_FAILURE;
    }

    status = count_zero_bytes(mixed, sizeof mixed / sizeof mixed[0], NULL);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "A null output pointer should be rejected.")) {
        return EXIT_FAILURE;
    }

    (void)fputs("c-006-unsigned-integers-and-byte-arrays-unit: PASS\n", stdout);
    return EXIT_SUCCESS;
}
