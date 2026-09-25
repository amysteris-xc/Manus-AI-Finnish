#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_varint(const uint8_t *in, size_t len, uint32_t *val, size_t *consumed)
{
    if (in == NULL || val == NULL || consumed == NULL || len == 0) return 0;
    uint32_t result = 0;
    size_t shift = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = in[i];
        if (shift >= 32) return 0;
        result |= (uint32_t)(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            *val = result;
            *consumed = i + 1;
            return 1;
        }
        shift += 7;
    }
    return 0;
}

int main(void)
{
    const uint8_t encoded[] = {0xAC, 0x02};
    uint32_t val = 0;
    size_t used = 0;
    if (!decode_varint(encoded, sizeof encoded, &val, &used) || val != 300) return EXIT_FAILURE;
    if (printf("Decoded: %" PRIu32 "\nConsumed: %zu\n", val, used) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
