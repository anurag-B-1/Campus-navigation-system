#ifndef ROUTER_H
#define ROUTER_H

#include "graph.h"
#include "structure.h"
#include <vector>
using namespace std;

#define MAX_NODES 50
#define INF 999999 // Represents an unreachable distance initially

/**
 * Calculates the shortest path between startNode and endNode using Dijkstra's
 * algorithm. Connected with Graph adjacency list, raw edge lists, and vector
 * edge representations.
 */

// Primary router function utilizing Graph adjacency list for neighbor traversal
PathResult findShortestPath(int startNode, int endNode, const Graph &graph,
                            const vector<Edge> &edgeList);

// Backward-compatible router function using raw array pointer
PathResult findShortestPath(int startNode, int endNode, const Edge *edgeList,
                            int edgeCount);

// Convenient overload directly accepting vector<Edge>
PathResult findShortestPath(int startNode, int endNode,
                            const vector<Edge> &edgeList);

#endif // ROUTER_H
