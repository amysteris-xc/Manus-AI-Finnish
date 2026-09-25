#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_colon(const uint8_t *buf, size_t len, size_t *colon_idx)
{
    if (buf == NULL || colon_idx == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == (uint8_t)':') {
            *colon_idx = i;
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    const uint8_t record[] = {'k', 'e', 'y', ':', 'v', 'a', 'l'};
    size_t colon_pos = 0;
    if (!find_colon(record, sizeof record, &colon_pos)) return EXIT_FAILURE;
    if (printf("Key length: %zu\nValue length: %zu\n", colon_pos, sizeof record - colon_pos - 1) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
