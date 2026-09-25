#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const uint8_t input[] = {UINT8_C(0),UINT8_C(0x41),UINT8_C(0xFF),UINT8_C(0x41),UINT8_C(0)};
    size_t counts[256] = {0};
    for (size_t index = 0; index < sizeof input / sizeof input[0]; ++index) ++counts[input[index]];
    if (printf("00: %zu\n41: %zu\nFF: %zu\n", counts[0], counts[UINT8_C(0x41)], counts[UINT8_C(0xFF)]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
