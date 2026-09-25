#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t expected; } SequenceState;

static int accept_byte(SequenceState *state, uint8_t byte)
{
    if (state == NULL || byte != state->expected) return 0;
    state->expected++;
    return 1;
}

int main(void)
{
    SequenceState state = {UINT8_C(1)};
    const uint8_t stream[] = {UINT8_C(1), UINT8_C(2), UINT8_C(3)};
    for (size_t i = 0; i < sizeof stream; ++i)
        if (!accept_byte(&state, stream[i])) return EXIT_FAILURE;
    if (printf("C 094 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
