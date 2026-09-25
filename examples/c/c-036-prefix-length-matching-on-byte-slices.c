#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t common_prefix(const uint8_t *a, size_t a_len, const uint8_t *b, size_t b_len)
{
    if (a == NULL || b == NULL) return 0;
    size_t limit = (a_len < b_len) ? a_len : b_len;
    size_t i = 0;
    while (i < limit && a[i] == b[i]) ++i;
    return i;
}

int main(void)
{
    const uint8_t b1[] = {1, 2, 3, 4};
    const uint8_t b2[] = {1, 2, 0, 4};
    size_t p = common_prefix(b1, sizeof b1, b2, sizeof b2);
    if (printf("Common prefix length: %zu\n", p) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
