#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int hex_val(uint8_t c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int decode_hex(const uint8_t *hex, size_t hex_len, uint8_t *out, size_t out_cap, size_t *out_len)
{
    if (hex == NULL || out == NULL || out_len == NULL || (hex_len % 2) != 0 || hex_len / 2 > out_cap) return 0;
    for (size_t i = 0; i < hex_len / 2; ++i) {
        int hi = hex_val(hex[i * 2]);
        int lo = hex_val(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0) return 0;
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    *out_len = hex_len / 2;
    return 1;
}

int main(void)
{
    const uint8_t hex[] = {'4', '1', '0', '0', '4', '2'};
    uint8_t bin[3];
    size_t written = 0;
    if (!decode_hex(hex, sizeof hex, bin, sizeof bin, &written)) return EXIT_FAILURE;
    if (printf("Bytes decoded: %zu\nFirst byte: 0x%02" PRIX8 "\n", written, bin[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
