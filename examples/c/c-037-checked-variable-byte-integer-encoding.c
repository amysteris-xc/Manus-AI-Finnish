#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode_varint(uint32_t val, uint8_t *out, size_t cap, size_t *used)
{
    if (out == NULL || used == NULL || cap == 0) return 0;
    size_t i = 0;
    while (val >= 0x80) {
        if (i >= cap) return 0;
        out[i++] = (uint8_t)((val & 0x7F) | 0x80);
        val >>= 7;
    }
    if (i >= cap) return 0;
    out[i++] = (uint8_t)(val & 0x7F);
    *used = i;
    return 1;
}

int main(void)
{
    uint8_t buf[5];
    size_t len = 0;
    if (!encode_varint(300, buf, sizeof buf, &len)) return EXIT_FAILURE;
    if (printf("Encoded bytes: %zu\n", len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
