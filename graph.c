#include "graph.h"

node_t *addNode(node_t *currentNode, char *nodeId) {
    bool isFirst = currentNode == NULL, isNodeInGraph;

    if (!isFirst) {
        isNodeInGraph = isInGraph(currentNode, nodeId);
    }

    if (isFirst || !isNodeInGraph) {
        node_t *temp = malloc(sizeof(node_t));
        temp->name = malloc(sizeof(char) * (strlen(nodeId) + 1));
        strcpy(temp->name, nodeId);
        temp->isVisited = false;
        temp->connectedNodes = NULL;
        temp->nextNodeInGraph = currentNode;
        return temp;
    }

    return currentNode;
}

node_t *addConnection(node_t *currentNode, char *nodeId, char *nodeToConnect, direction dir) {
    if (currentNode == NULL) {
        return NULL;
    }

    node_t *temp = malloc(sizeof(node_t));
    temp->name = currentNode->name;
    temp->isVisited = false;
    if (strcmp(currentNode->name, nodeId) == 0) {
        temp->connectedNodes = addListElement(currentNode->connectedNodes, nodeToConnect, dir);
        temp->nextNodeInGraph = currentNode->nextNodeInGraph;
        return temp;
    } else {
        temp->connectedNodes = currentNode->connectedNodes;
        temp->nextNodeInGraph = addConnection(currentNode->nextNodeInGraph, nodeId, nodeToConnect, dir);
        if (temp->nextNodeInGraph == NULL) {
            return currentNode;
        } else {
            return temp;
        }
    }
}

void printGraph(node_t *currentNode) {
    printf("%s: ", currentNode->name);
    printList(currentNode->connectedNodes);
    putchar('\n');
    if (currentNode->nextNodeInGraph != NULL) {
        printGraph(currentNode->nextNodeInGraph);
    }
}

bool isInGraph(node_t *currentNode, char *nodeId) {
    if (currentNode == NULL) {
        return false;
    } else if (strcmp(currentNode->name, nodeId) == 0) {
        return true;
    } else {
        return isInGraph(currentNode->nextNodeInGraph, nodeId);
    }
}

node_t *findNode(node_t *currentNode, char *nodeId) {
    if (currentNode == NULL) {
        return NULL;
    } else if (strcmp(currentNode->name, nodeId) == 0) {
        return currentNode;
    } else {
        findNode(currentNode->nextNodeInGraph, nodeId);
    }
}

void graphToDotUndir(node_t *graph, const char *fileName) {
    FILE *graphSrc = fopen(fileName, "w");

    fputs("graph gr {\n", graphSrc);

    set *processedNodes = NULL;

    node_t *temp = graph;
    while (temp != NULL) {
        fprintf(graphSrc, "\t%s", temp->name);
        list *currentConnection = temp->connectedNodes;
        if (currentConnection != NULL) {
            fputs(" -- ", graphSrc);
        }
        while (currentConnection != NULL) {
            if (!isInSet(processedNodes, currentConnection->value->value)) {
                fprintf(graphSrc, "%s", (char *) currentConnection->value->value);
                if (currentConnection->nextElement != NULL) {
                    fputs(", ", graphSrc);
                }
            }

            currentConnection = currentConnection->nextElement;
        }
        processedNodes = addSetElement(processedNodes, temp->name, STR_SET);
        fputc('\n', graphSrc);
        temp = temp->nextNodeInGraph;
    }
    putc('}', graphSrc);
    fclose(graphSrc);
}

void graphToDotDir(node_t *graph, const char *fileName) {
    FILE *graphSrc = fopen(fileName, "w");

    fputs("digraph gr {\n", graphSrc);

    set *processedNodes = NULL;

    node_t *temp = graph;
    while (temp != NULL) {
        list *currentConnection = temp->connectedNodes;
        if (currentConnection == NULL) {
            fprintf(graphSrc, "\t%s", temp->name);
        } else {
            while (currentConnection != NULL) {
                if (!isInSet(processedNodes, currentConnection->value->value)) {
                    fprintf(graphSrc, "\t%s -> %s ", (char*)temp->name, (char *) currentConnection->value->value);
                    if (currentConnection->value->dir == RIGHT) {
                        fputs("[dir=forward]\n", graphSrc);
                    } else {
                        fputs("[dir=back]\n", graphSrc);
                    }
                }

                currentConnection = currentConnection->nextElement;
            }
        }

        processedNodes = addSetElement(processedNodes, temp->name, STR_SET);
        fputc('\n', graphSrc);
        temp = temp->nextNodeInGraph;
    }
    putc('}', graphSrc);
    fclose(graphSrc);
}

node_t *graphFromFile(char *fileName) {
    // Opening source file
    FILE *input = fopen(fileName, "r");

    // File len
    fseek(input, 0, SEEK_END);
    int len = ftell(input);
    fseek(input, 0, SEEK_SET);
    char inputBuf[len];

    // Reading from file
    fread(inputBuf, sizeof(char), len, input);

    // Closing the file
    fclose(input);

    // Parsing the file
    node_t *tmpNode = NULL;
    char *currentEdgeBuf = strtok(inputBuf, "\n");
    while (currentEdgeBuf != NULL) {
        char *node = malloc(sizeof(char) * 4), *nodeToConnect = malloc(sizeof(char) * 4);
        sscanf(currentEdgeBuf, "%s %s", node, nodeToConnect);
        tmpNode = addNode(tmpNode, node);
        tmpNode = addConnection(tmpNode, node, nodeToConnect, RIGHT);
        tmpNode = addNode(tmpNode, nodeToConnect);
        tmpNode = addConnection(tmpNode, nodeToConnect, node, LEFT);
        currentEdgeBuf = strtok(NULL, "\n");
        free(node);
        free(nodeToConnect);
    }


    return tmpNode;
}

void freeGraph(node_t *graph) {
    node_t *currentNode = graph;
    while (currentNode != NULL) {
        node_t *nextNode = currentNode->nextNodeInGraph;
        free(currentNode->name);
        freeList(currentNode->connectedNodes);
        free(currentNode);
        currentNode = nextNode;
    }
}

int countConnectedNodes(node_t *graph) {
    if (graph != NULL) {
        if (graph->isVisited) {
            return 0;
        } else {
            int sum = 1;
            list *nextNode = graph->connectedNodes;
            graph->isVisited = true;
            while (nextNode != NULL) {
                sum += countConnectedNodes(findNode(graph, (char *) (nextNode->value->value)));
                nextNode = nextNode->nextElement;
            }
            return sum;
        }
    }
    return 0;
}

int countNodes(node_t *graph) {
    int nodesTotal = 0;
    if (graph != NULL) {
        node_t *currentNode = graph;
        while (currentNode != NULL) {
            nodesTotal++;
            currentNode = currentNode->nextNodeInGraph;
        }
        return nodesTotal;
    }
    return nodesTotal;
}


node_t *setUnvisited(node_t *graph) {
    if (graph == NULL) {
        return NULL;
    } else {
        node_t *temp = graph;
        temp->isVisited = false;
        temp->nextNodeInGraph = setUnvisited(graph->nextNodeInGraph);
        return temp;
    }
}