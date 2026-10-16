#include "../include/router.h"
#include <cstdlib>
#include <cstring>

// Primary Dijkstra implementation using Graph's linked list adjacency list and dynamic arrays
PathResult findShortestPath(int startNode, int endNode, const Graph& graph,
                            const Edge* edgeList, int edgeCount) {
    PathResult result;
    result.path_exists = false;
    result.total_weight = 0;
    result.node_sequence = nullptr;
    result.sequence_count = 0;
    result.path_descriptions = nullptr;
    result.description_count = 0;

    if (startNode < 0 || startNode >= MAX_NODES || endNode < 0 || endNode >= MAX_NODES) {
        return result;
    }

    if (startNode == endNode) {
        result.path_exists = true;
        result.total_weight = 0;
        result.node_sequence = (int*)std::malloc(sizeof(int));
        if (result.node_sequence) {
            result.node_sequence[0] = startNode;
            result.sequence_count = 1;
        }
        return result;
    }

    int dist[MAX_NODES];       // Shortest known distance to each node
    int parent[MAX_NODES];     // Previous node to trace back the path
    bool visited[MAX_NODES];   // True if shortest distance is finalized

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

        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;
        if (u == endNode) break; // Reached destination

        // Relax edges leaving node 'u' using Graph's linked list getNeighbors
        EdgeNode* edge = graph.getNeighbors(u);
        while (edge != nullptr) {
            int v = edge->to_node;
            int weight = edge->weight;

            if (v >= 0 && v < MAX_NODES && !visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            edge = edge->next;
        }
    }

    // 3. Reconstruct the final path if a route was found
    if (dist[endNode] != INF) {
        result.path_exists = true;
        result.total_weight = dist[endNode];

        // Temporary array to trace path backwards
        int tempPath[MAX_NODES];
        int pathLen = 0;
        int curr = endNode;

        while (curr != -1 && pathLen < MAX_NODES) {
            tempPath[pathLen++] = curr;
            curr = parent[curr];
        }

        // Allocate dynamic array for node_sequence using malloc
        result.sequence_count = pathLen;
        result.node_sequence = (int*)std::malloc(pathLen * sizeof(int));
        if (result.node_sequence) {
            for (int i = 0; i < pathLen; i++) {
                result.node_sequence[i] = tempPath[pathLen - 1 - i]; // Reverse into forward order
            }
        }

        // Allocate dynamic array of string pointers for path_descriptions using calloc
        int legCount = pathLen - 1;
        result.description_count = legCount;

        if (legCount > 0) {
            result.path_descriptions = (char**)std::calloc(legCount, sizeof(char*));

            if (result.path_descriptions && edgeList && edgeCount > 0) {
                for (int i = 0; i < legCount; i++) {
                    int current_u = result.node_sequence[i];
                    int next_v = result.node_sequence[i + 1];
                    const char* foundDesc = nullptr;

                    for (int e = 0; e < edgeCount; e++) {
                        if (edgeList[e].from_node == current_u && edgeList[e].to_node == next_v) {
                            foundDesc = edgeList[e].description;
                            break;
                        }
                    }

                    if (!foundDesc) {
                        foundDesc = "Proceed to the next waypoint.";
                    }

                    size_t len = std::strlen(foundDesc) + 1;
                    result.path_descriptions[i] = (char*)std::malloc(len);
                    if (result.path_descriptions[i]) {
                        std::memcpy(result.path_descriptions[i], foundDesc, len);
                    }
                }
            }
        }
    }

    return result;
}
