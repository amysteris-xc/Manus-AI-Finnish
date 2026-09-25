#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Deliberately insecure toy sponge pattern for education only */
static uint32_t toy_sponge(const uint8_t *data, size_t len)
{
    uint32_t state = 0xA5A5A5A5;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        state ^= data[i];
        state = (state << 5) | (state >> 27);
        state *= 0x5BD1E995;
    }
    return state;
}

int main(void)
{
    const uint8_t input[] = {'A', 'B', 'C'};
    uint32_t h = toy_sponge(input, sizeof input);
    if (printf("Toy sponge: 0x%08" PRIX32 "\n", h) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
