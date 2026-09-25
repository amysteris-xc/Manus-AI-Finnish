#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode(uint8_t out[], size_t capacity, const uint8_t payload[], size_t length, size_t *used)
{
    if (out == NULL || payload == NULL || used == NULL || length > UINT16_MAX || capacity < 2 || length > capacity - 2) return 0;
    out[0] = (uint8_t)(length >> 8); out[1] = (uint8_t)length;
    for (size_t index = 0; index < length; ++index) out[index + 2] = payload[index];
    *used = length + 2; return 1;
}
int main(void)
{
    const uint8_t payload[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    uint8_t frame[8]; size_t used;
    if (!encode(frame, sizeof frame, payload, sizeof payload, &used)) return EXIT_FAILURE;
    if (printf("Frame bytes: %zu\nLength: %u\n", used, (unsigned)(((unsigned)frame[0] << 8) | frame[1])) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
