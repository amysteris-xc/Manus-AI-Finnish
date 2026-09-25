#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int b64_encode3(const uint8_t in[3], char out[4])
{
    if (in == NULL || out == NULL) return 0;
    out[0] = b64_table[in[0] >> 2];
    out[1] = b64_table[((in[0] & 3) << 4) | (in[1] >> 4)];
    out[2] = b64_table[((in[1] & 0x0F) << 2) | (in[2] >> 6)];
    out[3] = b64_table[in[2] & 0x3F];
    return 1;
}

int main(void)
{
    const uint8_t raw[3] = {'M', 'a', 'n'};
    char enc[5] = {0};
    if (!b64_encode3(raw, enc)) return EXIT_FAILURE;
    if (printf("Base64: %s\n", enc) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
