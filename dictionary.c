// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void nFree(node *n);

// Number of buckets in hash table
const unsigned int N = 10;

//track word count
int tot_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Obtain hash value for given word
    int tray = hash(word);

    //create a pointer to a node and set it equal to the correct tray's linked list
    node *n = table[tray];


    while (n != NULL)
    {
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        else
        {
            n = n->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function taken from http://www.cse.yorku.ca/~oz/hash.html
    // unsigned long hash = 5381;
    // int c;

    // while ((c = *word++))
    // {
    //     hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    // }
    // return (int)(hash % N);

    //Alternate simple hash function utilizing first 3 letters of each word
    int hash = 0;
    int digit = 0;
    int j = 0;

    if (strlen(word) == 1)
    {
        j = 1;
    }
    else if (strlen(word) == 2)
    {
        j = 2;
    }
    else
    {
        j = 3;
    }

    for (int i = j; i >= 0; i--)
    {

        if ((int)word[i] == 39)
        {
            digit = 26;
        }
        else
        {
            digit = tolower((int)word[i]) - 97;
        }

        hash += digit * pow(27, i);
    }

    return (hash % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO

    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }
    char buff[LENGTH + 1];

    while (fscanf(dic, "%s", buff) != EOF)//(fgets(buff, LENGTH + 2, dic))
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Could not allocate memory for node.\n");
            return false;
        }

        strcpy(n->word, buff);
        n->next = NULL;

        tot_count++;

        //Call hash function to determine where in the hash table our new node goes
        int tray = hash(buff);

        if (table[tray] == NULL)
        {
            table[tray] = n;
        }
        else
        {
            n->next = table[tray];
            table[tray] = n;
        }
    }

    fclose(dic); //Close dictionary input file

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Return count corresponding to loaded dictionary size
    return tot_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO

    for (int i = 0; i < N; i++)
    {
        node *n = table[i];

        if (n != NULL)
        {
            nFree(n);
            free(table[i]);
        }
        else
        {
            free(table[i]);
        }
    }


    return true;
}

void nFree(node *n)
{
    if (n != NULL)
    {
        n = n->next;
        nFree(n);
        free(n);
    }
    else
    {
        free(n);
    }

}
