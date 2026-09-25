#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational accidental-corruption check only; not authentication or cryptography. */
static uint8_t educational_xor_check(const uint8_t *data, size_t count)
{
    uint8_t value = 0;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < count; ++i) value ^= data[i];
    return value;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(186), UINT8_C(1), UINT8_C(2)};
    uint8_t value = educational_xor_check(data, sizeof data);
    if (printf("C 186 educational check: %02" PRIX8 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
