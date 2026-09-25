#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x80) crc = (uint8_t)((crc << 1) ^ 0x07);
            else crc = (uint8_t)(crc << 1);
        }
    }
    return crc;
}

int main(void)
{
    const uint8_t test[] = {'1', '2', '3', '4', '5'};
    uint8_t val = crc8(test, sizeof test);
    if (printf("CRC-8: 0x%02" PRIX8 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
