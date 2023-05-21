#ifndef FLITA2_LIST_H
#define FLITA2_LIST_H

#include <stdbool.h>

typedef struct list list;
typedef enum direction direction;

#include "graph.h"

enum direction {
    RIGHT,
    LEFT
};

typedef enum type {
    INT,
    STR
} type;

typedef struct pair {
    char *value;
    direction dir;
} pair;

struct list {
    pair *value;
    struct list *nextElement;
};


// Checks if element is in list
bool isInList(list *pList, void *element);

// Adds element [newElement] to list
list *addListElement(list *pList, char *newElement, direction dir);

// Removes element [element] from list
list *removeListElement(list *pList, char *element);

// Prints all list elements
void printList(list *pList);

// Free list
void freeList(list *pList);

#endif