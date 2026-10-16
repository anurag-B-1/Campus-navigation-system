#include "../include/graph.h"

Graph::Graph() {
    for (int i = 0; i < MAX_GRAPH_NODES; i++) {
        adjList[i].head = nullptr;
    }
}

Graph::~Graph() {
    clear();
}

void Graph::clear() {
    for (int i = 0; i < MAX_GRAPH_NODES; i++) {
        if (adjList[i].head != nullptr) {
            freeEdgeList(adjList[i].head);
            adjList[i].head = nullptr;
        }
    }
}

void Graph::buildGraph(const Edge* rawEdges, int edgeCount) {
    clear();
    if (!rawEdges || edgeCount <= 0) return;

    for (int i = 0; i < edgeCount; i++) {
        int u = rawEdges[i].from_node;
        if (u >= 0 && u < MAX_GRAPH_NODES) {
            addEdgeToAdjList(adjList[u], rawEdges[i].edge_id, rawEdges[i].from_node,
                            rawEdges[i].to_node, rawEdges[i].weight, rawEdges[i].description);
        }
    }
}

EdgeNode* Graph::getNeighbors(int nodeID) const {
    if (nodeID >= 0 && nodeID < MAX_GRAPH_NODES) {
        return adjList[nodeID].head;
    }
    return nullptr;
}

bool Graph::hasNode(int nodeID) const {
    if (nodeID >= 0 && nodeID < MAX_GRAPH_NODES) {
        return adjList[nodeID].head != nullptr;
    }
    return false;
}
