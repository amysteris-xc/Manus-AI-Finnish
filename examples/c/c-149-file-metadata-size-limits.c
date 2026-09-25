#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_read_one(const char *path, uint8_t input, uint8_t *out)
{
    FILE *stream;
    if (path == NULL || out == NULL) return 0;
    stream = fopen(path, "wb");
    if (stream == NULL) return 0;
    if (fwrite(&input, sizeof input, 1, stream) != 1 || fclose(stream) != 0) return 0;
    stream = fopen(path, "rb");
    if (stream == NULL) return 0;
    if (fread(out, sizeof *out, 1, stream) != 1 || fclose(stream) != 0) return 0;
    return 1;
}

int main(void)
{
    uint8_t value = 0;
    if (!write_read_one("lesson.bin", UINT8_C(149), &value)) return EXIT_FAILURE;
    if (printf("C 149 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
