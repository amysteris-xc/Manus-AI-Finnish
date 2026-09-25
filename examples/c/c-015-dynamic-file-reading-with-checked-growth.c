#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t length; size_t capacity; } Buffer;

static int append(Buffer *buffer, const uint8_t *source, size_t count)
{
    size_t needed;
    uint8_t *temporary;
    if (buffer == NULL || source == NULL || count == 0) return count == 0;
    if (SIZE_MAX - buffer->length < count) return 0;
    needed = buffer->length + count;
    if (needed > buffer->capacity) {
        size_t capacity = buffer->capacity == 0 ? 64 : buffer->capacity;
        while (capacity < needed) { if (capacity > SIZE_MAX / 2) return 0; capacity *= 2; }
        temporary = realloc(buffer->data, capacity * sizeof *buffer->data);
        if (temporary == NULL) return 0;
        buffer->data = temporary; buffer->capacity = capacity;
    }
    for (size_t index = 0; index < count; ++index) buffer->data[buffer->length + index] = source[index];
    buffer->length = needed;
    return 1;
}

int main(int argc, char *argv[])
{
    uint8_t chunk[32];
    Buffer buffer = { NULL, 0, 0 };
    FILE *in;
    if (argc != 2) return EXIT_FAILURE;
    in = fopen(argv[1], "rb"); if (in == NULL) return EXIT_FAILURE;
    for (;;) { size_t got = fread(chunk, 1, sizeof chunk, in); if (!append(&buffer, chunk, got)) { free(buffer.data); (void)fclose(in); return EXIT_FAILURE; } if (got < sizeof chunk) { if (ferror(in)) { free(buffer.data); (void)fclose(in); return EXIT_FAILURE; } break; } }
    if (fclose(in) != 0 || buffer.length == 0 || buffer.data == NULL) {
        free(buffer.data);
        return EXIT_FAILURE;
    }
    if (printf("Bytes loaded: %zu\nFirst byte: 0x%02" PRIX8 "\nLast byte: 0x%02" PRIX8 "\n", buffer.length, buffer.data[0], buffer.data[buffer.length - 1]) < 0) { free(buffer.data); return EXIT_FAILURE; }
    free(buffer.data);
    return EXIT_SUCCESS;
}
