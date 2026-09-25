#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_tlv(const uint8_t *frame, size_t len, uint8_t *tag, const uint8_t **val, size_t *val_len)
{
    if (frame == NULL || tag == NULL || val == NULL || val_len == NULL || len < 2) return 0;
    *tag = frame[0];
    size_t l = frame[1];
    if (len - 2 < l) return 0;
    *val = frame + 2;
    *val_len = l;
    return 1;
}

int main(void)
{
    const uint8_t raw[] = {0x05, 0x02, 0xAA, 0xBB};
    uint8_t tag = 0;
    const uint8_t *val = NULL;
    size_t val_len = 0;
    if (!read_tlv(raw, sizeof raw, &tag, &val, &val_len)) return EXIT_FAILURE;
    if (printf("Tag: %" PRIu8 "\nVal length: %zu\n", tag, val_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
