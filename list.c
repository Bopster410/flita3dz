#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

list *addListElement(list *pList, char *newElement, direction dir) {
    bool isFirstElement = pList == NULL, isInListEl;
    if (!isFirstElement) {
        isInListEl = isInList(pList, newElement);
    }
    if (isFirstElement || !isInListEl) {
        list *pTemp = malloc(sizeof(list));
        pTemp->value = malloc(sizeof(pair));
        pTemp->value->value = malloc(sizeof(char) * (strlen(newElement) + 1));
        strcpy(pTemp->value->value, newElement);
        pTemp->value->dir = dir;
        pTemp->nextElement = pList;
        return pTemp;
    }
    return pList;
}

list *removeListElement(list *pList, char *element) {
    list *nextElement = pList->nextElement;
    if (pList != NULL) {
        if (strcmp(pList->value->value, element) == 0) {
            pList = pList->nextElement;
            return pList;
        } else if (strcmp(nextElement->value->value, element) == 0) {
            pList->nextElement = nextElement->nextElement;
            free(nextElement->value->value);
            free(nextElement->value);
            free(nextElement);
            return pList;
        }
    } else if (nextElement->nextElement == NULL) {
        return pList;
    } else {
        pList->nextElement = removeListElement(pList->nextElement, element);
        return pList;
    }
}

void printList(list *pList) {
    if (pList != NULL) {
        char *formatStr;
        switch (pList->value->dir) {
            case RIGHT:
                formatStr = "%s -> ";
                break;
            case LEFT:
                formatStr = "%s <- ";
                break;
            default:
                formatStr = "error\n";
        }
        if (pList->nextElement == NULL) {
            printf(formatStr, pList->value->value);
        } else {
            printf(formatStr, pList->value->value);
            printList(pList->nextElement);
        }
    } else {
        printf("{}");
    }

}

bool isInList(list *pList, void *element) {
    bool isElement = strcmp(pList->value->value, element) == 0;

    if (isElement) {
        return true;
    } else if (pList->nextElement == NULL) {
        return false;
    } else {
        return isInList(pList->nextElement, element);
    }
}

void freeList(list *pList) {
    if (pList != NULL) {
        if (pList->nextElement != NULL) {
            freeList(pList->nextElement);
        }
        free(pList->value->value);
        free(pList->value);
        free(pList);
    }
}