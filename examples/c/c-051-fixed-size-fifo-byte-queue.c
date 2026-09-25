#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t buf[16]; size_t r; size_t w; size_t n; } FIFO;

static int fifo_write(FIFO *f, const uint8_t *src, size_t len)
{
    if (f == NULL || src == NULL || len > 16 - f->n) return 0;
    for (size_t i = 0; i < len; ++i) {
        f->buf[f->w] = src[i];
        f->w = (f->w + 1) % 16;
        f->n++;
    }
    return 1;
}

int main(void)
{
    FIFO f = {{0}, 0, 0, 0};
    const uint8_t chunk[] = {1, 2, 3};
    if (!fifo_write(&f, chunk, sizeof chunk)) return EXIT_FAILURE;
    if (printf("Queued bytes: %zu\n", f.n) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
