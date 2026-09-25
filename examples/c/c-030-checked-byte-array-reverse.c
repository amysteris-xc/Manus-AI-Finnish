#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void reverse_bytes(uint8_t *buf, size_t len)
{
    if (buf == NULL || len < 2) return;
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
}

int main(void)
{
    uint8_t data[] = {1, 2, 3, 4, 5};
    reverse_bytes(data, sizeof data);
    if (printf("Reversed: %u %u %u %u %u\n", data[0], data[1], data[2], data[3], data[4]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
