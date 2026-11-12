#ifndef ROUTER_H
#define ROUTER_H

#include "graph.h"
#include "structure.h"

#define MAX_NODES MAX_GRAPH_NODES

/**
 * Calculates the shortest path between startNode and endNode using Dijkstra's
 * algorithm with dynamic arrays (malloc/calloc) and linked list neighbor relaxation.
 */
PathResult findShortestPath(int startNode, int endNode, const Graph& graph,
                            const Edge* edgeList, int edgeCount);

#endif // ROUTER_H
