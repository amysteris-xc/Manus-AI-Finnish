#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t crc8_fast(const uint8_t *data, size_t len)
{
    /* Minimal direct implementation matching 0x07 poly for verification */
    uint8_t crc = 0;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) crc = (uint8_t)((crc & 0x80) ? ((crc << 1) ^ 0x07) : (crc << 1));
    }
    return crc;
}

int main(void)
{
    const uint8_t test[] = {'A', 'B', 'C'};
    uint8_t val = crc8_fast(test, sizeof test);
    if (printf("Fast CRC-8: 0x%02" PRIX8 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
