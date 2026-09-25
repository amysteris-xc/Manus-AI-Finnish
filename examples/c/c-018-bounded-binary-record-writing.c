#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_all(FILE *out, const uint8_t *data, size_t count)
{
    size_t done = 0;
    while (done < count) {
        size_t step = fwrite(data + done, 1, count - done, out);
        if (step == 0) return 0;
        done += step;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    const uint8_t records[][8] = {{0,0,0,42,0,5,1,0},{0,0,1,35,0,160,2,1}};
    FILE *out;
    if (argc != 2) return EXIT_FAILURE;
    out = fopen(argv[1], "wb");
    if (out == NULL) return EXIT_FAILURE;
    for (size_t index = 0; index < sizeof records / sizeof records[0]; ++index)
        if (!write_all(out, records[index], sizeof records[index])) { (void)fclose(out); return EXIT_FAILURE; }
    {
        int flush_status = fflush(out);
        int close_status = fclose(out);

        if (flush_status != 0 || close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Records written: %zu\n", sizeof records / sizeof records[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
