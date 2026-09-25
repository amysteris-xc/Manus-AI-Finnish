#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t length; size_t capacity; } ByteVector;

static int append(ByteVector *vector, uint8_t value)
{
    if (vector == NULL) return 0;
    if (vector->length == vector->capacity) {
        size_t new_capacity = vector->capacity == 0 ? 4 : vector->capacity * 2;
        uint8_t *temporary;
        if (new_capacity < vector->capacity || new_capacity > SIZE_MAX / sizeof *vector->data) return 0;
        temporary = realloc(vector->data, new_capacity * sizeof *vector->data);
        if (temporary == NULL) return 0;
        vector->data = temporary;
        vector->capacity = new_capacity;
    }
    vector->data[vector->length++] = value;
    return 1;
}

int main(void)
{
    const uint8_t input[] = { UINT8_C(0x41), UINT8_C(0x00), UINT8_C(0xA7), UINT8_C(0x42), UINT8_C(0x7F) };
    ByteVector vector = { NULL, 0, 0 };
    for (size_t index = 0; index < sizeof input / sizeof input[0]; ++index) if (!append(&vector, input[index])) { free(vector.data); return EXIT_FAILURE; }
    if (printf("Length: %zu\nBytes:", vector.length) < 0) { free(vector.data); return EXIT_FAILURE; }
    for (size_t index = 0; index < vector.length; ++index) if (printf(" %02" PRIX8, vector.data[index]) < 0) { free(vector.data); return EXIT_FAILURE; }
    if (printf("\nCapacity: %zu\n", vector.capacity) < 0) { free(vector.data); return EXIT_FAILURE; }
    free(vector.data);
    return EXIT_SUCCESS;
}
