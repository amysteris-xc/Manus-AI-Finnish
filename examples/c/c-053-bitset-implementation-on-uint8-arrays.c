#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void bitset_set(uint8_t *bits, size_t cap_bytes, size_t bit)
{
    if (bits == NULL || bit / 8 >= cap_bytes) return;
    bits[bit / 8] |= (uint8_t)(1 << (bit % 8));
}

static int bitset_test(const uint8_t *bits, size_t cap_bytes, size_t bit)
{
    if (bits == NULL || bit / 8 >= cap_bytes) return 0;
    return (bits[bit / 8] & (1 << (bit % 8))) != 0;
}

int main(void)
{
    uint8_t set[2] = {0};
    bitset_set(set, sizeof set, 10);
    if (printf("Bit 10: %d\nBit 11: %d\n", bitset_test(set, sizeof set, 10), bitset_test(set, sizeof set, 11)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
