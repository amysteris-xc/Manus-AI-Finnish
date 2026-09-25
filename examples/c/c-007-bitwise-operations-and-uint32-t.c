#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(UINT32_MAX)
#error "This program requires an exact 32-bit uint32_t type."
#endif

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_VALUE_OUT_OF_RANGE = 2,
    STATUS_OUTPUT_FAILURE = 3
} Status;

enum {
    FLAGS_SHIFT = 8,
    SEQUENCE_SHIFT = 16
};

#define CATEGORY_MASK UINT32_C(0x000000FF)
#define SEQUENCE_VALUE_MASK UINT32_C(0x0000FFFF)

static Status pack_metadata(uint32_t category, uint32_t flags,
                            uint32_t sequence, uint32_t *word_out)
{
    if (word_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (category > CATEGORY_MASK || flags > CATEGORY_MASK ||
        sequence > SEQUENCE_VALUE_MASK) {
        return STATUS_VALUE_OUT_OF_RANGE;
    }

    *word_out = category | (flags << FLAGS_SHIFT) |
                (sequence << SEQUENCE_SHIFT);
    return STATUS_OK;
}

static Status unpack_metadata(uint32_t word, uint32_t *category_out,
                              uint32_t *flags_out, uint32_t *sequence_out)
{
    if (category_out == NULL || flags_out == NULL || sequence_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    *category_out = word & CATEGORY_MASK;
    *flags_out = (word >> FLAGS_SHIFT) & CATEGORY_MASK;
    *sequence_out = (word >> SEQUENCE_SHIFT) & SEQUENCE_VALUE_MASK;
    return STATUS_OK;
}

static Status write_word_report(const uint32_t words[], size_t count)
{
    if (words == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Word count: %zu\n", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        uint32_t category;
        uint32_t flags;
        uint32_t sequence;
        Status status = unpack_metadata(words[index], &category, &flags,
                                        &sequence);

        if (status != STATUS_OK) {
            return status;
        }

        if (printf("Word %zu: 0x%08" PRIX32
                   " category=%" PRIu32 " flags=%" PRIu32
                   " sequence=%" PRIu32 "\n",
                   index, words[index], category, flags, sequence) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    return STATUS_OK;
}

int main(void)
{
    uint32_t words[2];
    Status status;

    status = pack_metadata(UINT32_C(42), UINT32_C(5), UINT32_C(291),
                           &words[0]);
    if (status != STATUS_OK) {
        (void)fputs("Cannot pack the first metadata word.\n", stderr);
        return EXIT_FAILURE;
    }

    status = pack_metadata(UINT32_C(16), UINT32_C(160), UINT32_C(48879),
                           &words[1]);
    if (status != STATUS_OK) {
        (void)fputs("Cannot pack the second metadata word.\n", stderr);
        return EXIT_FAILURE;
    }

    status = write_word_report(words, sizeof words / sizeof words[0]);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
