#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int start_size, end_size;
    // TODO: Prompt for start size
    do
    {
        start_size = get_int("Start size: ");
    }
    while (start_size < 9);

    // TODO: Prompt for end size
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    while (start_size < end_size)
    {
        start_size += start_size / 3 - start_size / 4;
        year++;
    }

    // TODO: Print number of years
    printf("Years: %i", year);

}
