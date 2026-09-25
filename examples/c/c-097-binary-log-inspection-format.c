#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex(const uint8_t *data, size_t len)
{
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(0), UINT8_C(97), UINT8_C(255)};
    if (printf("C 097 hex: ") < 0 || !print_hex(data, sizeof data) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
