#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int b64_idx(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static int b64_decode4(const char in[4], uint8_t out[3])
{
    int i0 = b64_idx(in[0]), i1 = b64_idx(in[1]), i2 = b64_idx(in[2]), i3 = b64_idx(in[3]);
    if (i0 < 0 || i1 < 0 || i2 < 0 || i3 < 0) return 0;
    out[0] = (uint8_t)((i0 << 2) | (i1 >> 4));
    out[1] = (uint8_t)(((i1 & 0x0F) << 4) | (i2 >> 2));
    out[2] = (uint8_t)(((i2 & 3) << 6) | i3);
    return 1;
}

int main(void)
{
    uint8_t dec[3] = {0};
    if (!b64_decode4("TWFu", dec)) return EXIT_FAILURE;
    if (printf("Decoded: %c%c%c\n", dec[0], dec[1], dec[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
