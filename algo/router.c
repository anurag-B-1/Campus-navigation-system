#include "router.h"
#include <stdbool.h>

struct PathResult findShortestPath(int startNode, int endNode, const struct Edge* edgeList, int edgeCount) {
    struct PathResult result;
    result.path_exists = false;
    result.total_weight = 0;
    
    // Arrays needed for Dijkstra's Algorithm
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
        // Find the unvisited node with the minimum distance
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

        // If we reached our destination early, we can stop optimizing
        if (u == endNode) break;

        // Relax edges leaving node 'u'
        for (int i = 0; i < edgeCount; i++) {
            if (edgeList[i].from_node == u) {
                int v = edgeList[i].to_node;
                int weight = edgeList[i].weight;

                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // 3. Reconstruct the final path if a route was found
    if (dist[endNode] != INF) {
        result.path_exists = true;
        result.total_weight = dist[endNode];

        // Temporary array to store path backwards
        int tempPath[MAX_NODES];
        int pathLen = 0;
        int curr = endNode;

        while (curr != -1) {
            tempPath[pathLen++] = curr;
            curr = parent[curr];
        }

        // Standard C handles vectors inside the struct by utilizing C++ vector mechanisms 
        // when compiled across layers. Let's populate the struct sequence forwards:
        #ifdef __cplusplus
        for (int i = pathLen - 1; i >= 0; i--) {
            result.node_sequence.push_back(tempPath[i]);
        }
        
        // Match descriptions for the path sequence segments
        for (size_t i = 0; i < result.node_sequence.size() - 1; i++) {
            int current_u = result.node_sequence[i];
            int next_v = result.node_sequence[i+1];
            for (int k = 0; k < edgeCount; k++) {
                if (edgeList[k].from_node == current_u && edgeList[k].to_node == next_v) {
                    result.path_descriptions.push_back(edgeList[k].description);
                    break;
                }
            }
        }
        #endif
    }

    return result;
}
