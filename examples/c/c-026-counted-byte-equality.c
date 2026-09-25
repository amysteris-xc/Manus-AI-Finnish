#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int equal(const uint8_t left[], const uint8_t right[], size_t count)
{
    if ((left == NULL || right == NULL) && count != 0) return 0;
    for (size_t index = 0; index < count; ++index) if (left[index] != right[index]) return 0;
    return 1;
}
int main(void)
{
    const uint8_t a[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    const uint8_t b[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    const uint8_t c[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x43)};
    if (printf("a equals b: %s\na equals c: %s\n", equal(a,b,sizeof a) ? "yes" : "no", equal(a,c,sizeof a) ? "yes" : "no") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
