#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int consume(const uint8_t chunk[], size_t count, size_t *total, uint32_t *sum)
{
    if ((chunk == NULL && count != 0) || total == NULL || sum == NULL || SIZE_MAX - *total < count) return 0;
    for (size_t index = 0; index < count; ++index) {
        if (UINT32_MAX - *sum < chunk[index]) return 0;
        *sum += chunk[index];
    }
    *total += count; return 1;
}
int main(void)
{
    const uint8_t first[] = {UINT8_C(1),UINT8_C(2)};
    const uint8_t second[] = {UINT8_C(3),UINT8_C(0)};
    size_t total = 0; uint32_t sum = 0;
    if (!consume(first, sizeof first, &total, &sum) || !consume(second, sizeof second, &total, &sum)) return EXIT_FAILURE;
    if (printf("Bytes: %zu\nSum: %" PRIu32 "\n", total, sum) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
