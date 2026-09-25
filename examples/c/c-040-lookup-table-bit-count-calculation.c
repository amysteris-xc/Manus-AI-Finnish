#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t count_set_bits(const uint8_t *data, size_t len)
{
    static const uint8_t nibble_bits[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
    if (data == NULL) return 0;
    size_t total = 0;
    for (size_t i = 0; i < len; ++i) {
        total += nibble_bits[data[i] & 0x0F];
        total += nibble_bits[(data[i] >> 4) & 0x0F];
    }
    return total;
}

int main(void)
{
    const uint8_t data[] = {0xFF, 0x01};
    size_t bits = count_set_bits(data, sizeof data);
    if (printf("Set bits: %zu\n", bits) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
