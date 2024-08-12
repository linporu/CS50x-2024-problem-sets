// Implements a dictionary's functionality
#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];


// Declare functions
bool load(const char *dictionary);
unsigned int hash(const char *word);
unsigned int size(void);
bool check(const char *word);
bool unload(void);

// Declare global variables
int word_counter = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Could not open.\n");
        return false;
    }

    // Read each word in the file
    char word[LENGTH + 1];
    int index = 0;
    while (fscanf(source, "%s", word) != EOF)
    {
        // Add each word to the hash table
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = table[hash(word)];
        table[hash(word)] = n;

        // Increment word counter
        word_counter++;
    }

    // Close the dictionary file
    fclose(source);
    return true;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash_value = 0;
    int prime_value[45] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197};

    for (int i = 0; word[i] != 0; i++)
    {
        hash_value += (tolower(word[i]) * prime_value[i]);
    }

    return hash_value % 1000;
}



// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Loop over the hash table
    for (node *ptr = table[hash(word)]; ptr != NULL; ptr = ptr->next)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop over all hash table
    for (int i = 0; i < N; i++)
    {
        // Free memory
        node *ptr = table[i];
        while (ptr != NULL)
        {
            ptr = table[i]->next;
            free(table[i]);
            table[i] = ptr;
        }
    }
    return true;
}
