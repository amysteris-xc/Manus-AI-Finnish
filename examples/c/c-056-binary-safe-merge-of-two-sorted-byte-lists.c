#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int merge_sorted(const uint8_t *a, size_t a_len, const uint8_t *b, size_t b_len, uint8_t *out, size_t out_cap)
{
    if ((a == NULL && a_len != 0) || (b == NULL && b_len != 0) || out == NULL) return 0;
    if (a_len > out_cap || b_len > out_cap - a_len) return 0;
    size_t i = 0, j = 0, k = 0;
    while (i < a_len && j < b_len) {
        if (a[i] <= b[j]) out[k++] = a[i++];
        else out[k++] = b[j++];
    }
    while (i < a_len) out[k++] = a[i++];
    while (j < b_len) out[k++] = b[j++];
    return 1;
}

int main(void)
{
    const uint8_t l1[] = {1, 4, 7};
    const uint8_t l2[] = {2, 5, 8};
    uint8_t res[6];
    if (!merge_sorted(l1, sizeof l1, l2, sizeof l2, res, sizeof res)) return EXIT_FAILURE;
    if (printf("Merged 6 items: %u %u ... %u\n", res[0], res[1], res[5]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
