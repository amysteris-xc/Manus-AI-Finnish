#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_bytes(const uint8_t *haystack, size_t h_len, const uint8_t *needle, size_t n_len, size_t *pos)
{
    if (haystack == NULL || needle == NULL || pos == NULL || n_len == 0 || n_len > h_len) return 0;
    for (size_t i = 0; i <= h_len - n_len; ++i) {
        int match = 1;
        for (size_t j = 0; j < n_len; ++j) {
            if (haystack[i + j] != needle[j]) { match = 0; break; }
        }
        if (match) { *pos = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t hay[] = {'a', 'b', 'c', 'd', 'e'};
    const uint8_t ndl[] = {'c', 'd'};
    size_t match_idx = 0;
    if (!find_bytes(hay, sizeof hay, ndl, sizeof ndl, &match_idx)) return EXIT_FAILURE;
    if (printf("Match at index: %zu\n", match_idx) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
