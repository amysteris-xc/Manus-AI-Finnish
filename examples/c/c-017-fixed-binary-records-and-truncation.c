#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t get_be32(const uint8_t in[4]) { return ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) | ((uint32_t)in[2] << 8) | in[3]; }
static uint16_t get_be16(const uint8_t in[2]) { return (uint16_t)(((uint16_t)in[0] << 8) | in[1]); }

int main(int argc, char *argv[])
{
    uint8_t record[8];
    size_t count = 0;
    FILE *in;
    if (argc != 2) return EXIT_FAILURE;
    in = fopen(argv[1], "rb"); if (in == NULL) return EXIT_FAILURE;
    for (;;) {
        size_t got = fread(record, 1, sizeof record, in);
        if (got == 0) { if (ferror(in)) { (void)fclose(in); return EXIT_FAILURE; } break; }
        if (got != sizeof record) { (void)fclose(in); (void)fputs("Truncated record.\n", stderr); return EXIT_FAILURE; }
        if (printf("Record %zu: id=%" PRIu32 " flags=%" PRIu16 " kind=%" PRIu8 " status=%" PRIu8 "\n", count, get_be32(record), get_be16(record + 4), record[6], record[7]) < 0) { (void)fclose(in); return EXIT_FAILURE; }
        if (count == SIZE_MAX) { (void)fclose(in); return EXIT_FAILURE; }
        ++count;
    }
    if (fclose(in) != 0) return EXIT_FAILURE;
    if (printf("Records: %zu\n", count) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
