#ifndef GRAPH_H
#define GRAPH_H

#include "structure.h"

#define MAX_GRAPH_NODES 100

class Graph {
private:
    AdjList adjList[MAX_GRAPH_NODES]; // Adjacency list: Array of Linked Lists (DSA in C)

public:
    Graph();
    ~Graph();

    // Builds the adjacency list from raw edges array
    void buildGraph(const Edge* rawEdges, int edgeCount);

    // Returns head of linked list of direct outgoing pathways
    EdgeNode* getNeighbors(int nodeID) const;

    // Checks if a node has any registered outgoing connections
    bool hasNode(int nodeID) const;

    // Clears all linked lists and frees memory
    void clear();
};

#endif // GRAPH_H
