#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t type; uint16_t length; } Header;
static int decode_header(const uint8_t bytes[], size_t count, Header *out)
{
    if (bytes == NULL || out == NULL || count < 3) return 0;
    out->type = bytes[0]; out->length = (uint16_t)(((uint16_t)bytes[1] << 8) | bytes[2]);
    return 1;
}
int main(void)
{
    const uint8_t frame[] = {UINT8_C(2),UINT8_C(0),UINT8_C(3),UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    Header header;
    if (!decode_header(frame, sizeof frame, &header) || header.length != sizeof frame - 3) return EXIT_FAILURE;
    if (printf("Type: %" PRIu8 "\nPayload length: %" PRIu16 "\n", header.type, header.length) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
