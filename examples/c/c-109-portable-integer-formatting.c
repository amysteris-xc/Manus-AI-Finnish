#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex_prefix(const uint8_t *data, size_t count, size_t maximum)
{
    size_t shown = count < maximum ? count : maximum;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < shown; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(109), UINT8_C(255)};
    if (printf("C 109 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
