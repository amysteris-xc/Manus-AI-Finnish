#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    const uint8_t record[] = {UINT8_C(0x48),UINT8_C(0x45),UINT8_C(0x4C),UINT8_C(0x4C),UINT8_C(0x4F)};
    FILE *out;
    if (argc != 2) return EXIT_FAILURE;
    out = fopen(argv[1], "ab");
    if (out == NULL) return EXIT_FAILURE;
    if (fwrite(record, 1, sizeof record, out) != sizeof record) {
        (void)fclose(out);
        return EXIT_FAILURE;
    }
    {
        int flush_status = fflush(out);
        int close_status = fclose(out);

        if (flush_status != 0 || close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Bytes appended: %zu\n", sizeof record) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
