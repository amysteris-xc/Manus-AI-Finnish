#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t capacity; size_t position; } Writer;

static int writer_put(Writer *writer, uint8_t value)
{
    if (writer == NULL || writer->data == NULL || writer->position >= writer->capacity) return 0;
    writer->data[writer->position] = value;
    writer->position++;
    return 1;
}

int main(void)
{
    uint8_t bytes[2] = {0};
    Writer writer = {bytes, sizeof bytes, 0};
    if (!writer_put(&writer, UINT8_C(199))) return EXIT_FAILURE;
    if (printf("C 199 writer byte: %u\n", (unsigned)bytes[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
