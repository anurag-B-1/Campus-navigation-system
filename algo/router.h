#ifndef ROUTER_H
#define ROUTER_H

#include "structure.h"

#define MAX_NODES 50  // Increased to 50 to cover new nodes like Central Library
#define INF 999999    // Represents an unreachable distance initially

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 👤 TASK FOR MEMBER 3:
 * Calculates the shortest path between startNode and endNode.
 * It uses standard arrays to trace back the absolute shortest distance.
 */
struct PathResult findShortestPath(int startNode, int endNode, const struct Edge* edgeList, int edgeCount);

#ifdef __cplusplus
}
#endif

#endif // ROUTER_H
