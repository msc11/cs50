// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

// custom: to keep track of the number of words
unsigned int wcount = 0;
// custom: prototype of free_recursive
void free_recursive(node *n);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int idx = hash(word);

    node *cursor = table[idx];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int sum = 0;
    int i = 0;
    while (word[i] != '\0')
    {
        sum += word[i];
        i += 1;
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        printf("cannot open the file\n");
        return false;
    }

    char nextword[LENGTH + 1];
    while (fscanf(input, "%s", nextword) != EOF)
    {
        node *n = malloc(sizeof(node));
        strcpy(n->word, nextword);

        int idx = hash(nextword);
        n->next = table[idx];
        table[idx] = n;

        wcount++;
    }

    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        free_recursive(table[i]);
    }

    return true;
}

void free_recursive(node *n)
{
    // custom: to free nodes recursively
    if (n == NULL)
    {
        free(n);
        return;
    }
    else
    {
        free_recursive(n->next);
    }

    free(n);
}
