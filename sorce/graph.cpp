#include "../include/graph.h"

using namespace std;

void Graph::buildGraph(const vector<Edge>& rawEdges) {
    adjList.clear();
    for (const auto& edge : rawEdges) {
        // Organize edges by their starting point ('from_node')
        adjList[edge.from_node].push_back(edge);
    }
}

std::vector<Edge> Graph::getNeighbors(int nodeID) const {
    // If the node exists in our map, return its connections
    auto it = adjList.find(nodeID);
    if (it != adjList.end()) {
        return it->second;
    }
    return vector<Edge>(); // Return empty list if no connections found
}

bool Graph::hasNode(int nodeID) const {
    return adjList.find(nodeID) != adjList.end();
}

const std::map<int, std::vector<Edge>>& Graph::getAdjList() const {
    return adjList;
}
