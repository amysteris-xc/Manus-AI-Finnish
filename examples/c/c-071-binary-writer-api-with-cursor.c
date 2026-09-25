#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_u16_be(const uint8_t *data, size_t len, uint16_t *out)
{
    if (data == NULL || out == NULL || len < 2) return 0;
    *out = (uint16_t)(((uint16_t)data[0] << 8) | data[1]);
    return 1;
}

int main(void)
{
    const uint8_t field[] = {UINT8_C(0), UINT8_C(71)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 071 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
