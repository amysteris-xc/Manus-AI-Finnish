#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational only: this is not a cryptographic hash, MAC, cipher, or security control. */
static uint32_t toy_mix32(const uint8_t bytes[], size_t count)
{
    uint32_t state = UINT32_C(0x12345678);
    for (size_t index = 0; index < count; ++index) state = (state ^ bytes[index]) * UINT32_C(33) + UINT32_C(0x9E3779B9);
    return state;
}
int main(void)
{
    const uint8_t vector[] = {UINT8_C(1),UINT8_C(2),UINT8_C(3)};
    const uint32_t result = toy_mix32(vector, sizeof vector);
    if (printf("Toy mix: 0x%08" PRIX32 "\n", result) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
