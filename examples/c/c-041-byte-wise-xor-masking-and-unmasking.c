#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void apply_mask(uint8_t *data, size_t len, const uint8_t *key, size_t key_len)
{
    if (data == NULL || key == NULL || key_len == 0) return;
    for (size_t i = 0; i < len; ++i) data[i] ^= key[i % key_len];
}

int main(void)
{
    uint8_t msg[] = {'H', 'E', 'L', 'L', 'O'};
    const uint8_t key[] = {0xAA, 0x55};
    apply_mask(msg, sizeof msg, key, sizeof key);
    apply_mask(msg, sizeof msg, key, sizeof key);
    if (printf("Restored: %c%c%c%c%c\n", msg[0], msg[1], msg[2], msg[3], msg[4]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
