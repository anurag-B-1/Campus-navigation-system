#include "graph.h"
using namespace std;

void Graph::buildGraph(const vector<Edge>& rawEdges) {
    for (const auto& edge : rawEdges) {
        // Organize edges by their starting point ('from_node')
        adjList[edge.from_node].push_back(edge);
    }
}

std::vector<Edge> Graph::getNeighbors(int nodeID) {
    // If the node exists in our map, return its connections
    if (adjList.find(nodeID) != adjList.end()) {
        return adjList[nodeID];
    }
    return vector<Edge>(); // Return empty list if no connections found
}
