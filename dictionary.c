// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5381;

// Number of words in dictionary
unsigned int dictionaryWords = 0;

// Hash table
node *table[N];

unsigned int hashCode = 0;
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Convert word to lowercase to get same key from hash function
    char copy[strlen(word) + 1];
    strcpy(copy, word);
    char *p = copy;
    for (; *p; ++p)
    {
        *p = tolower(*p);
    }

    unsigned int key = hash(copy);

    // Temp traversal pointer (points to head)
    node *trav = table[key];

    // Traverse through linked list (linear search)
    while (trav != NULL)
    {
        if (strcmp(copy, trav->word) == 0)
        {
            return true;
        }
        trav = trav->next;
    }
    return false;
}


// Hashes word to a number
unsigned int hash(const char *w)
{
    unsigned long hash = 5381;
    int C;

    while ((C = tolower(*w++)))
    {
        hash = ((hash << 5) + hash) + C; 
    }
    return hash % N;
}
bool load(const char *dictionary)
{
    char word[LENGTH + 1];
    FILE *dic = fopen(dictionary, "r");
    if (dic != NULL)
    {
        while (fscanf(dic, "%s", word) != EOF)
        {
            node *n = malloc(sizeof(node));

            if (n != NULL)
            {
                hashCode = hash(word);
                strcpy(n->word, word);
                n->next = table[hashCode];
                table[hashCode] = n;
                // free(n);
                dictionaryWords ++;
            }

        }
        fclose(dic);
    }
    else
    {
        return false;
    }
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionaryWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}