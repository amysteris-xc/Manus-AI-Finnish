#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_u64_be(const uint8_t *in, size_t len, uint64_t *out)
{
    if (in == NULL || out == NULL || len < 8) return 0;
    uint64_t v = 0;
    for (size_t i = 0; i < 8; ++i) v = (v << 8) | in[i];
    *out = v;
    return 1;
}

int main(void)
{
    const uint8_t raw[8] = {0, 0, 0, 0, 0, 0, 1, 0};
    uint64_t val = 0;
    if (!decode_u64_be(raw, sizeof raw, &val)) return EXIT_FAILURE;
    if (printf("Decoded u64: %" PRIu64 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
