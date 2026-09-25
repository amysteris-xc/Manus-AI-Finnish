#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational integrity check only: not a MAC or cryptographic hash. */
static uint32_t educational_crc(const uint8_t *data, size_t len)
{
    uint32_t crc = UINT32_C(0xFFFFFFFF);
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ ((crc & 1U) ? UINT32_C(0xEDB88320) : 0U);
    }
    return ~crc;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(1), UINT8_C(2), UINT8_C(3)};
    uint32_t value = educational_crc(data, sizeof data);
    if (printf("C 086 educational check: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
