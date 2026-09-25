#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode_u64_be(uint64_t v, uint8_t *out, size_t cap)
{
    if (out == NULL || cap < 8) return 0;
    for (int i = 7; i >= 0; --i) {
        out[i] = (uint8_t)(v & 0xFF);
        v >>= 8;
    }
    return 1;
}

int main(void)
{
    uint8_t buf[8];
    if (!encode_u64_be(0x0102030405060708ULL, buf, sizeof buf)) return EXIT_FAILURE;
    if (printf("Encoded byte 0: %u\nEncoded byte 7: %u\n", buf[0], buf[7]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
