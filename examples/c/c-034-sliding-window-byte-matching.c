#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const uint8_t stream[] = {10, 20, 30, 40, 50};
    const size_t win = 3;
    uint32_t sum = 0;
    for (size_t i = 0; i < win; ++i) sum += stream[i];
    uint32_t first_sum = sum;
    sum = sum - stream[0] + stream[win];
    if (printf("First window: %" PRIu32 "\nSecond window: %" PRIu32 "\n", first_sum, sum) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
