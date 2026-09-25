#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational only: demonstrates constant-time loop pattern */
static int ct_equal(const uint8_t *a, const uint8_t *b, size_t len)
{
    if (a == NULL || b == NULL) return 0;
    uint8_t diff = 0;
    for (size_t i = 0; i < len; ++i) diff |= (uint8_t)(a[i] ^ b[i]);
    return diff == 0;
}

int main(void)
{
    const uint8_t k1[4] = {1, 2, 3, 4};
    const uint8_t k2[4] = {1, 2, 3, 4};
    const uint8_t k3[4] = {1, 2, 9, 4};
    if (printf("k1 == k2: %d\nk1 == k3: %d\n", ct_equal(k1, k2, 4), ct_equal(k1, k3, 4)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
