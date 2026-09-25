#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int copy_bytes(uint8_t out[], size_t out_count, const uint8_t in[], size_t count)
{
    if ((out == NULL || in == NULL) && count != 0) return 0;
    if (count > out_count) return 0;
    for (size_t index = 0; index < count; ++index) out[index] = in[index];
    return 1;
}
static int equal_bytes(const uint8_t a[], const uint8_t b[], size_t count)
{
    if ((a == NULL || b == NULL) && count != 0) return 0;
    for (size_t index = 0; index < count; ++index) if (a[index] != b[index]) return 0;
    return 1;
}
int main(void)
{
    const uint8_t source[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    uint8_t copy[3] = {0};
    if (!copy_bytes(copy, sizeof copy, source, sizeof source) || !equal_bytes(source, copy, sizeof source)) return EXIT_FAILURE;
    if (printf("Copied bytes: %zu\nEqual: yes\n", sizeof source) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
