#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_u16_be(const uint8_t *bytes, size_t count, uint16_t *out)
{
    if (bytes == NULL || out == NULL || count != 2) return 0;
    *out = (uint16_t)(((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1]);
    return 1;
}

int main(void)
{
    const uint8_t encoded[] = {UINT8_C(0), UINT8_C(103)};
    uint16_t value = 0;
    if (!decode_u16_be(encoded, sizeof encoded, &value)) return EXIT_FAILURE;
    if (printf("C 103 canonical value: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
