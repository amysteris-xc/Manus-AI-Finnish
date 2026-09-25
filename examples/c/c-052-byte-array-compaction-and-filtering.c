#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t filter_byte(uint8_t *buf, size_t len, uint8_t reject)
{
    if (buf == NULL) return 0;
    size_t w = 0;
    for (size_t r = 0; r < len; ++r) {
        if (buf[r] != reject) buf[w++] = buf[r];
    }
    return w;
}

int main(void)
{
    uint8_t data[] = {'a', ' ', 'b', ' ', 'c'};
    size_t compact = filter_byte(data, sizeof data, ' ');
    if (printf("Filtered length: %zu\n", compact) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
