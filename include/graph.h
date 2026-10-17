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

  
    void buildGraph(const Edge* rawEdges, int edgeCount);

    
    EdgeNode* getNeighbors(int nodeID) const;

    bool hasNode(int nodeID) const;

    void clear();
};

#endif // GRAPH_H
