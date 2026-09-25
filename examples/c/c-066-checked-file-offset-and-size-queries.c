#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_and_read_demo(const char *path, uint8_t *out)
{
    const uint8_t value = UINT8_C(67);
    FILE *stream = fopen(path, "wb");
    if (stream == NULL) return 0;
    if (fwrite(&value, sizeof value, 1, stream) != 1 || fclose(stream) != 0) return 0;
    stream = fopen(path, "rb");
    if (stream == NULL) return 0;
    if (fread(out, sizeof *out, 1, stream) != 1 || fclose(stream) != 0) return 0;
    return 1;
}

int main(void)
{
    uint8_t value = 0;
    if (!write_and_read_demo("lesson.bin", &value)) return EXIT_FAILURE;
    if (printf("C 066 verified file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
