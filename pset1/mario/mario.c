#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = -1;
    while (height < 1 || height > 8)
    {
        printf("Height:");
        scanf("%d", &height);
    }

    for (int i = 1; i <= height; i++)
    {
        printf("%.*s", height - i, "        ");
        printf("%.*s", i, "########");
        printf("  ");
        printf("%.*s\n", i, "########");
    }
}

