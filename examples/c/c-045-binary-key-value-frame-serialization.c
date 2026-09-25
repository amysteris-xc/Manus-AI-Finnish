#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_tlv(uint8_t tag, const uint8_t *val, uint8_t val_len, uint8_t *out, size_t cap, size_t *written)
{
    if (val == NULL && val_len != 0) return 0;
    if (out == NULL || written == NULL || cap < (size_t)(val_len + 2)) return 0;
    out[0] = tag;
    out[1] = val_len;
    for (size_t i = 0; i < val_len; ++i) out[2 + i] = val[i];
    *written = val_len + 2;
    return 1;
}

int main(void)
{
    uint8_t frame[10];
    const uint8_t payload[] = {0xDE, 0xAD};
    size_t used = 0;
    if (!write_tlv(0x01, payload, sizeof payload, frame, sizeof frame, &used)) return EXIT_FAILURE;
    if (printf("TLV written: %zu bytes\n", used) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
