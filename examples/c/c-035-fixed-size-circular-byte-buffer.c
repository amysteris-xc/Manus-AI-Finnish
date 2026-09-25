#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t data[4]; size_t head; size_t tail; size_t count; } Ring;

static int ring_push(Ring *r, uint8_t b)
{
    if (r == NULL || r->count >= 4) return 0;
    r->data[r->head] = b;
    r->head = (r->head + 1) % 4;
    r->count++;
    return 1;
}

static int ring_pop(Ring *r, uint8_t *b)
{
    if (r == NULL || b == NULL || r->count == 0) return 0;
    *b = r->data[r->tail];
    r->tail = (r->tail + 1) % 4;
    r->count--;
    return 1;
}

int main(void)
{
    Ring r = {{0}, 0, 0, 0};
    uint8_t out = 0;
    if (!ring_push(&r, 42) || !ring_push(&r, 99)) return EXIT_FAILURE;
    if (!ring_pop(&r, &out) || out != 42) return EXIT_FAILURE;
    if (printf("Popped: %u\nRemaining: %zu\n", out, r.count) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
