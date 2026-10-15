#include "../include/router.h"
#include <vector>
#include <string>

using namespace std;

// Primary Dijkstra implementation using Graph's adjacency list
PathResult findShortestPath(int startNode, int endNode, const Graph& graph, const vector<Edge>& edgeList) {
    PathResult result;
    result.path_exists = false;
    result.total_weight = 0;

    if (startNode < 0 || startNode >= MAX_NODES || endNode < 0 || endNode >= MAX_NODES) {
        return result;
    }

    if (startNode == endNode) {
        result.path_exists = true;
        result.total_weight = 0;
        result.node_sequence.push_back(startNode);
        return result;
    }

    int dist[MAX_NODES];       // Stores the shortest known distance to each node
    int parent[MAX_NODES];     // Stores the previous node to trace back the path
    bool visited[MAX_NODES];   // Tracks if a node's shortest distance is finalized

    // 1. Initialization
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }
    dist[startNode] = 0;

    // 2. Core Dijkstra Loop
    for (int count = 0; count < MAX_NODES - 1; count++) {
        int minDist = INF;
        int u = -1;

        for (int v = 0; v < MAX_NODES; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        // If we can't find a valid next node or reached an isolated part, stop
        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;

        // If we reached our destination early, stop optimizing
        if (u == endNode) break;

        // Relax edges leaving node 'u' using Graph's getNeighbors
        vector<Edge> neighbors = graph.getNeighbors(u);
        for (const auto& edge : neighbors) {
            int v = edge.to_node;
            int weight = edge.weight;

            if (v >= 0 && v < MAX_NODES && !visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // 3. Reconstruct the final path if a route was found
    if (dist[endNode] != INF) {
        result.path_exists = true;
        result.total_weight = dist[endNode];

        // Temporary vector to store path backwards
        vector<int> tempPath;
        int curr = endNode;

        while (curr != -1) {
            tempPath.push_back(curr);
            curr = parent[curr];
        }

        // Populate node sequence in forward order
        for (int i = (int)tempPath.size() - 1; i >= 0; i--) {
            result.node_sequence.push_back(tempPath[i]);
        }

        // Match turn-by-turn descriptions for each leg of the path
        for (size_t i = 0; i < result.node_sequence.size() - 1; i++) {
            int current_u = result.node_sequence[i];
            int next_v = result.node_sequence[i + 1];
            bool foundDesc = false;

            for (const auto& edge : edgeList) {
                if (edge.from_node == current_u && edge.to_node == next_v) {
                    result.path_descriptions.push_back(edge.description);
                    foundDesc = true;
                    break;
                }
            }

            if (!foundDesc) {
                result.path_descriptions.push_back("Proceed to the next waypoint.");
            }
        }
    }

    return result;
}

// Convenient overload accepting std::vector<Edge> directly
PathResult findShortestPath(int startNode, int endNode, const vector<Edge>& edgeList) {
    Graph tempGraph;
    tempGraph.buildGraph(edgeList);
    return findShortestPath(startNode, endNode, tempGraph, edgeList);
}

// Backward-compatible overload accepting raw Edge array pointer
PathResult findShortestPath(int startNode, int endNode, const Edge* edgeList, int edgeCount) {
    vector<Edge> edges;
    if (edgeList && edgeCount > 0) {
        edges.assign(edgeList, edgeList + edgeCount);
    }
    return findShortestPath(startNode, endNode, edges);
}
