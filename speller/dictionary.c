#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dictionary.h"


// total size of hash table
#define SIZE 100000

// make the nodes for list
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

// make hashtable
node* hashtable[SIZE] = {NULL};

// make hash function
int hash (const char* word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // check  if word[i] is letter
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;


        else
            n = 27;
           //left shift
            hash = ((hash << 3) + n) % SIZE;
    }
    return hash;
}

// create global variable to count size
int dictionarySize = 0;

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    // opens dictionary
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
        return false;
        fprintf(stderr, "File does not exist!\n");
    // create an array for word to be stored in
    char word[LENGTH+1];

    // scan through the file, get each word into the hash table
    while (fscanf(file, "%s\n", word)!= EOF)
    {

        dictionarySize++;

        // get memory value for new word
        node* newWord = malloc(sizeof(node));

        // put word in the new node
        strcpy(newWord->word, word);

        // look for array index for word should go
        int index = hash(word);

        // if index of hash table empty
        if (hashtable[index] == NULL)
        {
            hashtable[index] = newWord;
            newWord->next = NULL;
        }

        // C
        else
        {
            newWord->next = hashtable[index];
            hashtable[index] = newWord;
        }
    }


    fclose(file);

    // if successful
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    // make variable for lower-cased  words
    char temp[LENGTH + 1];
    int len = strlen(word);
    for(int i = 0; i < len; i++)
        temp[i] = tolower(word[i]);
    temp[len] = '\0';

    // look for array index for word should go
    int index = hash(temp);

    // if index of hash table empty
    if (hashtable[index] == NULL)
    {
        return false;
    }

    // make the cursor to compare words
    node* cursor = hashtable[index];

    // if hash table is not empty at index, iterate through words and compare
    while (cursor != NULL)
    {
        if (strcmp(temp, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    // if you don't find the word
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    // if dictionary is loaded, return number of words
    if (dictionarySize > 0)
    {
        return dictionarySize;
    }

    // if dictionary hasn't been loaded
    else
        return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    // create a variable to go through index
    int index = 0;

    // iterate through entire hashtable array
    while (index < SIZE)
    {
        // if hashtable is empty at index, go to next index
        if (hashtable[index] == NULL)
        {
            index++;
        }

        // if hashtable is not empty, iterate through nodes and start freeing
        else
        {
            while(hashtable[index] != NULL)
            {
                node* cursor = hashtable[index];
                hashtable[index] = cursor->next;
                free(cursor);
            }

            // once hashtable is empty at index, go to next index
            index++;
        }
    }

    // return true if successful
    return true;
}