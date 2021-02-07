#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BSIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("It should include one argument\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = NULL;

    if (input == NULL)
    {
        printf("There is something wrong with the file\n");
        return 1;
    }

    BYTE buf[BSIZE];
    int count = 0;
    char filename[8];
    while (fread(buf, BSIZE, 1, input))
    {
        //printf("%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3] & 0xf0);
        if ((buf[0] == 0xff) && (buf[1] == 0xd8) && (buf[2] == 0xff) && ((buf[3] & 0xf0) == 0xe0))
        {
            if (count != 0)
            {
                fclose(output);
            }

            sprintf(filename, "%03d.jpg", count);
            output = fopen(filename, "w");
            fwrite(buf, BSIZE, 1, output);

            count++;
        }
        else if (count > 0)
        {
            fwrite(buf, BSIZE, 1, output);
        }
    }

    fclose(input);
}