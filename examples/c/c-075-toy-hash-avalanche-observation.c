#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Deliberately non-cryptographic teaching hash; never use for real security. */
static uint32_t toy_fnv1a(const uint8_t *data, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        hash ^= data[i];
        hash *= UINT32_C(16777619);
    }
    return hash;
}

int main(void)
{
    const uint8_t data[] = {'L', '0', '1'};
    uint32_t value = toy_fnv1a(data, sizeof data);
    if (printf("C 075 toy hash: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
