#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct node node_t;

struct node {
    node_t *nextNodeInGraph;
    list *connectedNodes;
    char *name;
    bool isVisited;
};


node_t *addNode(node_t *currentNode, char *nodeId);

node_t *addConnection(node_t *currentNode, char *nodeId, char *nodeToConnect, direction dir);

bool isInGraph(node_t *currentNode, char *nodeId);

void printGraph(node_t *firstNode);

node_t *findNode(node_t *currentNode, char *nodeId);

void graphToDotUndir(node_t *graph, const char *fileName);

void graphToDotDir(node_t *graph, const char *fileName);

node_t *graphFromFile(char *fileName);

void freeGraph(node_t *graph);

int countConnectedNodes(node_t *graph);

int countNodes(node_t *graph);

node_t *setUnvisited(node_t *graph);

#endif