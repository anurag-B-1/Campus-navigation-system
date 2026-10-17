#ifndef SEARCH_H
#define SEARCH_H

#include "graph.h"
#include "structure.h"

#define SEARCH_MAX_NODES 100

struct IntList {
  int *items;
  int count;
};

void freeIntList(IntList *list);

struct TraversalResult {
  int *items;
  int *hops;
  int count;
};

void freeTraversalResult(TraversalResult *result);

TraversalResult bfsTraversal(const Graph &graph, int startNode);

TraversalResult dfsTraversal(const Graph &graph, int startNode);

IntList searchLocations(const Node *nodes, int nodeCount, const char *query);

IntList findBfsPath(const Graph &graph, int startNode, int destNode);

IntList findIntermediateDestinations(const Graph &graph, int startNode,
                                     int destNode);

#endif /* SEARCH_H */
