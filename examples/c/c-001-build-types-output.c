#include <stdio.h>

int main(void)
{
    const int lesson_number = 1;
    const double planned_minutes = 30.0;
    const char track_initial = 'C';

    if (printf("Track: %c\n", track_initial) < 0) {
        return 1;
    }

    if (printf("Lesson: %d\n", lesson_number) < 0) {
        return 1;
    }

    if (printf("Planned study time: %.0f minutes\n", planned_minutes) < 0) {
        return 1;
    }

    return 0;
}
