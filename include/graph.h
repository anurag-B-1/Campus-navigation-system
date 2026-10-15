#ifndef GRAPH_H
#define GRAPH_H

#include "structure.h"
#include <vector>
#include <map>

class Graph {
private:
    std::map<int, std::vector<Edge>> adjList;     // Adjacency list: nodeID -> outward Edges

public:
    void buildGraph(const std::vector<Edge>& rawEdges);

    // Returns all direct paths leading out of a specific location
    std::vector<Edge> getNeighbors(int nodeID) const;

    // Checks if a node has any registered outgoing connections
    bool hasNode(int nodeID) const;

    // Returns read-only reference to adjacency list
    const std::map<int, std::vector<Edge>>& getAdjList() const;
};

#endif // GRAPH_H
