#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint8_t buffer[64];
    size_t bytes = 0, zeros = 0, high = 0;
    FILE *in;
    if (argc != 2) { (void)fputs("Usage: stats INPUT\n", stderr); return EXIT_FAILURE; }
    in = fopen(argv[1], "rb");
    if (in == NULL) return EXIT_FAILURE;
    for (;;) {
        size_t got = fread(buffer, 1, sizeof buffer, in);
        if (SIZE_MAX - bytes < got) { (void)fclose(in); return EXIT_FAILURE; }
        for (size_t index = 0; index < got; ++index) {
            if (buffer[index] == UINT8_C(0)) ++zeros;
            if (buffer[index] >= UINT8_C(0x80)) ++high;
        }
        bytes += got;
        if (got < sizeof buffer) { if (ferror(in)) { (void)fclose(in); return EXIT_FAILURE; } break; }
    }
    if (fclose(in) != 0) return EXIT_FAILURE;
    if (printf("Bytes: %zu\nZero bytes: %zu\nHigh bytes: %zu\n", bytes, zeros, high) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
