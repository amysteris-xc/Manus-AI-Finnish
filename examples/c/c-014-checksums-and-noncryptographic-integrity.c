#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t byte_sum32(const uint8_t bytes[], size_t count)
{
    uint32_t sum = UINT32_C(0);
    for (size_t index = 0; index < count; ++index) sum += bytes[index];
    return sum;
}

int main(void)
{
    const uint8_t vector[] = { UINT8_C(1), UINT8_C(2), UINT8_C(3), UINT8_C(4), UINT8_C(0) };
    const uint32_t result = byte_sum32(vector, sizeof vector / sizeof vector[0]);
    if (printf("Bytes: 5\nToy byte sum: 0x%08" PRIX32 "\n", result) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
