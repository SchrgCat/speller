/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "dictionary.h"
#include <string.h>

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define ALPHABET_SIZE (27)


typedef struct node {
    struct node *children[ALPHABET_SIZE];
    bool isLeaf;
} node;

node top = {.children = {NULL}, .isLeaf = false};
node *root = &top;
bool loaded = false;
int wordCount = 0;
int wasApostrophe = (int)'\'' - (int)'a';

struct node *getNode(void) {
    struct node *newNode = malloc(sizeof(node));
    int i;
    
    if (newNode) {
        newNode->isLeaf = false;
        
        for(i = 0; i < ALPHABET_SIZE; ++i) {
            newNode->children[i] = NULL;
        }
    }
    
    return newNode;
}

void insert(struct node *root, char *word) {
    int index;
    int length = strlen(word);
    int level;
    
    struct node *trav = root;
    
    for (level = 0; level < length; ++level) {
        
       index = CHAR_TO_INDEX(word[level]);
       
       if (index < 0) {
           index = 26;
       }
        
        if(!trav->children[index]) {
            trav->children[index] = getNode();
        }
        
        trav = trav->children[index];
    }
    
    trav->isLeaf = true;
} 

void freenode (node *trav) {
    
    for (int j = 0; j < ALPHABET_SIZE; j++) {
        if(trav->children[j] != NULL) {
            freenode(trav->children[j]);
        }
    }
    
    free(trav);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *trav = root;
    int length = strlen(word);
    int index;
    for (int level = 0; level < length; level++) {
       
       index = CHAR_TO_INDEX(word[level]);
        
        if (index < 0) {
            if (index == wasApostrophe) {
                index = 26;
            } else {
                index += 32;      
            }
        } 
        
        if(!trav->children[index]) {
            return false;
        } else {
            
            trav = trav->children[index];
        }
    }
    
    
    return ((trav != NULL) && trav->isLeaf);
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *dictPtr = fopen(dictionary, "r");
    char word[46];
    char c;
    int index = 0;
    
    for (c = fgetc(dictPtr); c != EOF; c = fgetc(dictPtr)) {
        if (c == '\n') {
            word[index] = '\0';
            insert(root, word);
            index = 0;
            ++wordCount;
        } else {
            word[index] = c;
            ++index;
        }  
    }
    
    if (c == EOF) {
        loaded = true;
        return true;
    } else {
        return false;
    } 
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded) {
        return wordCount;
    } else {
        return 0;
    }
}

/**a
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
   int i;
   for (i = 0; i < ALPHABET_SIZE; i++) {
       if (root->children[i] != NULL) {
            freenode(root->children[i]);
       }
   }
   
    return true;
}