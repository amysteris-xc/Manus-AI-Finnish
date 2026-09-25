#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_span(const uint8_t *source, size_t count, uint8_t *destination, size_t capacity)
{
    if ((source == NULL && count != 0) || destination == NULL || count > capacity) return 0;
    if (count != 0) memmove(destination, source, count);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(133), UINT8_C(0)};
    uint8_t destination[2] = {0};
    if (!copy_span(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 133 copied byte: %u\n", (unsigned)destination[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
