#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int get_index(string);
void print_grade(int);

int main(void)
{
    string text = get_string("Text:");
    int index = get_index(text);
    print_grade(index);
}

int get_index(string text)
{
    const char *ptext = text;
    int word_cnt = 0;
    int letter_cnt = 0;
    int sentence_cnt = 0;

    for (int i = 0; i < strlen(text); i++, ptext++)
    {
        if (isalpha(*ptext))
        {
            letter_cnt += 1;
        }
        else
        {
            if (*ptext == ' ')
            {
                word_cnt += 1;
            }
            else if (*ptext == '.' || *ptext == '?' || *ptext == '!')
            {
                sentence_cnt += 1;

                if (i == strlen(text) - 1)
                {
                    word_cnt += 1;
                }
            }
        }
    }

    float L = (float) letter_cnt / word_cnt * 100;
    float S = (float) sentence_cnt / word_cnt * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    return (round(index));
}

void print_grade(int index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}
