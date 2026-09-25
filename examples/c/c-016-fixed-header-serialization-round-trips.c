#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void put_be16(uint8_t out[2], uint16_t value) { out[0] = (uint8_t)(value >> 8); out[1] = (uint8_t)value; }
static uint16_t get_be16(const uint8_t in[2]) { return (uint16_t)(((uint16_t)in[0] << 8) | in[1]); }
static void put_be32(uint8_t out[4], uint32_t value) { out[0] = (uint8_t)(value >> 24); out[1] = (uint8_t)(value >> 16); out[2] = (uint8_t)(value >> 8); out[3] = (uint8_t)value; }
static uint32_t get_be32(const uint8_t in[4]) { return ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) | ((uint32_t)in[2] << 8) | in[3]; }

int main(void)
{
    uint8_t header[8];
    const uint16_t version = UINT16_C(3), flags = UINT16_C(5);
    const uint32_t length = UINT32_C(291);
    put_be16(header, version); put_be16(header + 2, flags); put_be32(header + 4, length);
    if (get_be16(header) != version || get_be16(header + 2) != flags || get_be32(header + 4) != length) return EXIT_FAILURE;
    if (printf("Header bytes: %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 "\nVersion: %" PRIu16 " Flags: %" PRIu16 " Length: %" PRIu32 "\n", header[0], header[1], header[2], header[3], header[4], header[5], header[6], header[7], version, flags, length) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
