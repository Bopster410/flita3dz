#ifndef FLITA2_SET_H
#define FLITA2_SET_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum typeSet {
    INT_SET,
    STR_SET
} typeSet;

typedef struct set {
    void *value;
    struct set *nextElement;
    typeSet valueType;
} set;


// Adds element [newElement] to set
set* addSetElement(set *pSet, void *newElement, typeSet valueType);

// Removes element [element] from set
set* removeSetElement(set *pSet, void *element);

// Prints all set elements
void printSet(set *pSet);

// Checks if element is in set
bool isInSet(set *pSet, void *element);

void freeSet(set *pSet);

#endif