#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATE_MAGIC, STATE_LEN, STATE_PAYLOAD, STATE_DONE, STATE_ERROR } ParserState;

typedef struct {
    ParserState state;
    size_t expected_len;
    size_t read_len;
    uint8_t payload[8];
} ProtocolParser;

static void parser_feed(ProtocolParser *p, uint8_t byte)
{
    switch (p->state) {
    case STATE_MAGIC:
        p->state = (byte == 0xAA) ? STATE_LEN : STATE_ERROR;
        break;
    case STATE_LEN:
        if (byte > sizeof p->payload) p->state = STATE_ERROR;
        else {
            p->expected_len = byte;
            p->read_len = 0;
            p->state = (byte == 0) ? STATE_DONE : STATE_PAYLOAD;
        }
        break;
    case STATE_PAYLOAD:
        p->payload[p->read_len++] = byte;
        if (p->read_len == p->expected_len) p->state = STATE_DONE;
        break;
    default:
        break;
    }
}

int main(void)
{
    ProtocolParser p = {STATE_MAGIC, 0, 0, {0}};
    const uint8_t stream[] = {0xAA, 0x02, 0x10, 0x20};
    for (size_t i = 0; i < sizeof stream; ++i) parser_feed(&p, stream[i]);
    if (p.state != STATE_DONE) return EXIT_FAILURE;
    if (printf("Parser finished successfully with %zu payload bytes\n", p.read_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
