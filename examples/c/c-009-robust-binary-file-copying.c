#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int write_all(FILE *out, const uint8_t *buffer, size_t count)
{
    size_t written = 0;
    while (written < count) {
        size_t step = fwrite(buffer + written, 1, count - written, out);
        if (step == 0) return 0;
        written += step;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    uint8_t buffer[64];
    size_t total = 0;
    FILE *in;
    FILE *out;
    if (argc != 3) { (void)fputs("Usage: copy INPUT OUTPUT\n", stderr); return EXIT_FAILURE; }
    in = fopen(argv[1], "rb");
    if (in == NULL) { (void)fputs("Cannot open input.\n", stderr); return EXIT_FAILURE; }
    out = fopen(argv[2], "wb");
    if (out == NULL) { (void)fclose(in); (void)fputs("Cannot open output.\n", stderr); return EXIT_FAILURE; }
    for (;;) {
        size_t got = fread(buffer, 1, sizeof buffer, in);
        if (got > 0) {
            if (SIZE_MAX - total < got || !write_all(out, buffer, got)) { (void)fclose(in); (void)fclose(out); return EXIT_FAILURE; }
            total += got;
        }
        if (got < sizeof buffer) {
            if (ferror(in)) { (void)fclose(in); (void)fclose(out); return EXIT_FAILURE; }
            break;
        }
    }
    {
        int flush_status = fflush(out);
        int input_close_status = fclose(in);
        int output_close_status = fclose(out);

        if (flush_status != 0 || input_close_status != 0 ||
            output_close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Copied bytes: %zu\n", total) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
