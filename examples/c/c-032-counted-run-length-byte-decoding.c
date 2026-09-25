#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int rle_decode(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len)
{
    if (in == NULL || out == NULL || out_len == NULL || (in_len % 2) != 0) return 0;
    size_t written = 0;
    for (size_t i = 0; i < in_len; i += 2) {
        uint8_t count = in[i];
        uint8_t byte = in[i + 1];
        if (count > out_cap - written) return 0;
        for (uint8_t c = 0; c < count; ++c) out[written++] = byte;
    }
    *out_len = written;
    return 1;
}

int main(void)
{
    const uint8_t encoded[] = {3, 'A', 2, 'B'};
    uint8_t decoded[10];
    size_t total = 0;
    if (!rle_decode(encoded, sizeof encoded, decoded, sizeof decoded, &total)) return EXIT_FAILURE;
    if (printf("Decoded size: %zu\n", total) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
