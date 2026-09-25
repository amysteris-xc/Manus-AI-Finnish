#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *end = NULL;
    unsigned long parsed;
    uint32_t word;
    if (argc != 2) { (void)fputs("Usage: fields HEX_WORD\n", stderr); return EXIT_FAILURE; }
    errno = 0;
    parsed = strtoul(argv[1], &end, 0);
    if (errno != 0 || end == argv[1] || *end != '\0' || parsed > UINT32_MAX) { (void)fputs("Invalid uint32 value.\n", stderr); return EXIT_FAILURE; }
    word = (uint32_t)parsed;
    if (printf("Word: 0x%08" PRIX32 "\nCategory: %" PRIu32 "\nFlags: %" PRIu32 "\nSequence: %" PRIu32 "\n", word, word & UINT32_C(0xFF), (word >> 8) & UINT32_C(0xFF), (word >> 16) & UINT32_C(0xFFFF)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
