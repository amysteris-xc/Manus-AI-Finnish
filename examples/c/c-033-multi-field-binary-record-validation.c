#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int validate_record(const uint8_t *rec, size_t len)
{
    if (rec == NULL || len != 12) return 0;
    if (rec[0] != 'M' || rec[1] != 'A' || rec[2] != 'G' || rec[3] != 'C') return 0;
    uint16_t version = (uint16_t)(((uint16_t)rec[4] << 8) | rec[5]);
    if (version != 1) return 0;
    return 1;
}

int main(void)
{
    const uint8_t record[12] = {'M', 'A', 'G', 'C', 0, 1, 0, 0, 0, 0, 0, 0};
    if (!validate_record(record, sizeof record)) return EXIT_FAILURE;
    if (printf("Record valid: yes\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
