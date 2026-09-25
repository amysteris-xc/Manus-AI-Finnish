#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t calculate_parity(const uint8_t *data, size_t len)
{
    uint8_t p = 0;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t b = data[i];
        for (int bit = 0; bit < 8; ++bit) p ^= (uint8_t)((b >> bit) & 1);
    }
    return p;
}

int main(void)
{
    const uint8_t payload[] = {0x03, 0x05};
    uint8_t par = calculate_parity(payload, sizeof payload);
    if (printf("Parity bit: %u\n", par) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
