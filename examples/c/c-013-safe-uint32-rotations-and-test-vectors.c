#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int rotate_left32(uint32_t input, unsigned int count, uint32_t *out)
{
    if (out == NULL || count >= 32U) return 0;
    if (count == 0U) { *out = input; return 1; }
    *out = (input << count) | (input >> (32U - count));
    return 1;
}

int main(void)
{
    uint32_t first, second;
    if (!rotate_left32(UINT32_C(0x12345678), 8U, &first) || !rotate_left32(UINT32_C(0x80000001), 1U, &second)) return EXIT_FAILURE;
    if (printf("rotl32(0x12345678, 8) = 0x%08" PRIX32 "\nrotl32(0x80000001, 1) = 0x%08" PRIX32 "\n", first, second) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
