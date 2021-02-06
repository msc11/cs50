#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int validity_test(string key);
string encrypt(string);

char KEY[26];

int main(int argc, string argv[])
{
    // validity check for key argument
    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!validity_test(argv[1]))
    {
        printf("Key must only contain non-repeating alphabetic characters.\n");
        return 1;
    }

    // convert key to uppercase and save as a memeber
    for (int i = 0; i < 26; i++)
    {
        KEY[i] = toupper(argv[1][i]);
    }

    // get text from a user
    string plaintext = get_string("plaintext: ");

    // cipher text
    string ciphertext = encrypt(plaintext);
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

int validity_test(string key)
{
    int used[26] = {0};

    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            return 0;
        }
        else if (used[toupper(key[i]) - 'A'] != 0)
        {
            return 0;
        }
        else
        {
            used[toupper(key[i]) - 'A'] = 1;
        }
    }

    return 1;
}

string encrypt(string text)
{
    int islowercase = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (!isalpha(text[i]))
        {
            continue;
        }
        else if (islower(text[i]))
        {
            text[i] = tolower(KEY[toupper(text[i]) - 'A']);
        }
        else
        {
            text[i] = KEY[text[i] - 'A'];
        }
    }

    return (text);
}