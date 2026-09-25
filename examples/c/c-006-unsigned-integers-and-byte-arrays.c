#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(UINT8_MAX)
#error "This program requires an exact 8-bit uint8_t type."
#endif

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_OUTPUT_FAILURE = 2
} Status;

static Status count_zero_bytes(const uint8_t bytes[], size_t count,
                               size_t *zero_count_out)
{
    size_t zero_count = 0;

    if (bytes == NULL || count == 0 || zero_count_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    for (size_t index = 0; index < count; ++index) {
        if (bytes[index] == UINT8_C(0)) {
            ++zero_count;
        }
    }

    *zero_count_out = zero_count;
    return STATUS_OK;
}

static Status write_byte_report(const uint8_t bytes[], size_t count,
                                size_t zero_count)
{
    if (bytes == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Byte count: %zu\nBytes:", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        if (printf(" %02" PRIX8, bytes[index]) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    if (printf("\nZero-valued bytes: %zu\n", zero_count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

int main(void)
{
    const uint8_t payload[] = {
        UINT8_C(0x52), UINT8_C(0x00), UINT8_C(0x41),
        UINT8_C(0xA7), UINT8_C(0x00), UINT8_C(0x7F)
    };
    const size_t payload_count = sizeof payload / sizeof payload[0];
    size_t zero_count;
    Status status;

    status = count_zero_bytes(payload, payload_count, &zero_count);
    if (status != STATUS_OK) {
        (void)fputs("Cannot inspect the byte buffer.\n", stderr);
        return EXIT_FAILURE;
    }

    status = write_byte_report(payload, payload_count, zero_count);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
