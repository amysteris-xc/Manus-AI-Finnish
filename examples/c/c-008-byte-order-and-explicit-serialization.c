#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_INVALID_ARGUMENT = 1, STATUS_OUTPUT_FAILURE = 2 } Status;

static Status encode_be32(uint32_t value, uint8_t out[4])
{
    if (out == NULL) return STATUS_INVALID_ARGUMENT;
    out[0] = (uint8_t)(value >> 24);
    out[1] = (uint8_t)(value >> 16);
    out[2] = (uint8_t)(value >> 8);
    out[3] = (uint8_t)value;
    return STATUS_OK;
}

static Status decode_be32(const uint8_t in[4], uint32_t *value_out)
{
    if (in == NULL || value_out == NULL) return STATUS_INVALID_ARGUMENT;
    *value_out = ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) |
                 ((uint32_t)in[2] << 8) | (uint32_t)in[3];
    return STATUS_OK;
}

int main(void)
{
    const uint32_t input = UINT32_C(0x0123052A);
    uint8_t bytes[4];
    uint32_t decoded;
    if (encode_be32(input, bytes) != STATUS_OK || decode_be32(bytes, &decoded) != STATUS_OK) return EXIT_FAILURE;
    if (printf("Input word: 0x%08" PRIX32 "\nBig-endian bytes: %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 "\nDecoded word: 0x%08" PRIX32 "\n", input, bytes[0], bytes[1], bytes[2], bytes[3], decoded) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
