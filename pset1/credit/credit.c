#include <stdio.h>
#include <cs50.h>
#include <math.h>

string card(long num)
{
    int length = floor(log10(num)) + 1;
    int checksum = 0;
    string card_comp = "";

    for (int i = 1; i <= length; i ++)
    {
        if (i % 2 == 0)
        {
            int buf = 2 * (num % (long)pow(10, i) / (long)pow(10, i - 1));
            checksum = checksum + (buf / 10) + (buf % 10);
        }
        else
        {
            checksum += num % (long)pow(10, i) / (long)pow(10, i - 1);
        }
    }

    if (checksum % 10 != 0)
    {
        card_comp = "INVALID";
    }
    else
    {
        int code = num / (long)pow(10, i - 3);
        if (length == 15 && (code == 34 | code == 37))
        {
            card_comp = "AMEX";
        }
        else if (length == 16 && (code == 51 | code == 52 | code == 53 | code == 54 | code == 55))
        {
            card_comp = "MASTERCARD";
        }
        else if ((length == 13 | length == 16) && (code / 10 == 4))
        {
            card_comp = "VISA";
        }
        else
        {
            card_comp = "INVALID";
        }
    }

    return (card_comp);
}


int main(void)
{
    long number = get_long("Number:");
    printf("%s\n", card(number));
}

