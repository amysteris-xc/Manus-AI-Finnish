#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t dedup_adjacent(uint8_t *data, size_t len)
{
    if (data == NULL || len == 0) return 0;
    size_t w = 1;
    for (size_t r = 1; r < len; ++r) {
        if (data[r] != data[w - 1]) data[w++] = data[r];
    }
    return w;
}

int main(void)
{
    uint8_t stream[] = {1, 1, 2, 3, 3, 3, 4};
    size_t new_len = dedup_adjacent(stream, sizeof stream);
    if (printf("Deduped length: %zu\n", new_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
