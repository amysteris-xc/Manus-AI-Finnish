#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int checked_count_bytes(size_t count, size_t item_size, size_t *out)
{
    if (out == NULL || item_size == 0 || count > SIZE_MAX / item_size) return 0;
    *out = count * item_size;
    return 1;
}

int main(void)
{
    size_t bytes = 0;
    if (!checked_count_bytes((size_t)142, sizeof(uint8_t), &bytes)) return EXIT_FAILURE;
    if (printf("C 142 checked bytes: %zu\n", bytes) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
