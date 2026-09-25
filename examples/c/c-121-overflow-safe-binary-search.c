#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_byte(const uint8_t *data, size_t count, uint8_t target, size_t *out_index)
{
    if (data == NULL || out_index == NULL) return 0;
    for (size_t i = 0; i < count; ++i) {
        if (data[i] == target) { *out_index = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t table[] = {UINT8_C(1), UINT8_C(121), UINT8_C(250)};
    size_t index = 0;
    if (!find_byte(table, sizeof table, UINT8_C(121), &index)) return EXIT_FAILURE;
    if (printf("C 121 table index: %zu\n", index) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
