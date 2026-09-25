#define main c007_program_main
#include "../examples/c/c-007-bitwise-operations-and-uint32-t.c"
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
    uint32_t word = UINT32_C(0);
    uint32_t category = UINT32_C(0);
    uint32_t flags = UINT32_C(0);
    uint32_t sequence = UINT32_C(0);
    Status status;

    status = pack_metadata(UINT32_C(42), UINT32_C(5), UINT32_C(291), &word);
    if (!expect(status == STATUS_OK, "Valid fields should pack.") ||
        !expect(word == UINT32_C(0x0123052A),
                "Packed word should match its documented layout.")) {
        return EXIT_FAILURE;
    }

    status = unpack_metadata(word, &category, &flags, &sequence);
    if (!expect(status == STATUS_OK, "Valid outputs should unpack.") ||
        !expect(category == UINT32_C(42), "Category should be 42.") ||
        !expect(flags == UINT32_C(5), "Flags should be 5.") ||
        !expect(sequence == UINT32_C(291), "Sequence should be 291.")) {
        return EXIT_FAILURE;
    }

    word = UINT32_C(0xA5A5A5A5);
    status = pack_metadata(UINT32_C(256), UINT32_C(0), UINT32_C(0), &word);
    if (!expect(status == STATUS_VALUE_OUT_OF_RANGE,
                "A category outside eight bits should be rejected.") ||
        !expect(word == UINT32_C(0xA5A5A5A5),
                "Failed packing must not replace the output word.")) {
        return EXIT_FAILURE;
    }

    status = pack_metadata(UINT32_C(0), UINT32_C(256), UINT32_C(0), &word);
    if (!expect(status == STATUS_VALUE_OUT_OF_RANGE,
                "Flags outside eight bits should be rejected.")) {
        return EXIT_FAILURE;
    }

    status = pack_metadata(UINT32_C(0), UINT32_C(0), UINT32_C(65536), &word);
    if (!expect(status == STATUS_VALUE_OUT_OF_RANGE,
                "A sequence outside sixteen bits should be rejected.")) {
        return EXIT_FAILURE;
    }

    status = pack_metadata(UINT32_C(0), UINT32_C(0), UINT32_C(0), NULL);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "A null packing output pointer should be rejected.")) {
        return EXIT_FAILURE;
    }

    status = unpack_metadata(word, NULL, &flags, &sequence);
    if (!expect(status == STATUS_INVALID_ARGUMENT,
                "A null unpacking output pointer should be rejected.")) {
        return EXIT_FAILURE;
    }

    (void)fputs("c-007-bitwise-operations-and-uint32-t-unit: PASS\n", stdout);
    return EXIT_SUCCESS;
}
