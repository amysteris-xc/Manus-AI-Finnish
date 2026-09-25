#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int add_u32(uint32_t left, uint32_t right, uint32_t *out)
{
    if (out == NULL || UINT32_MAX - left < right) return 0;
    *out = left + right;
    return 1;
}
int main(void)
{
    uint32_t result;
    if (!add_u32(UINT32_C(4000000000), UINT32_C(12), &result)) return EXIT_FAILURE;
    if (printf("Sum: %" PRIu32 "\nOverflow rejected: %s\n", result, add_u32(UINT32_MAX, UINT32_C(1), &result) ? "no" : "yes") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
