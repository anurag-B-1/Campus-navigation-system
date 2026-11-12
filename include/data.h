#ifndef DATA_H
#define DATA_H

#include "structure.h"

// Loads the directed paths from data/connections.csv.
// Returns a dynamically allocated array, or nullptr when loading fails.
Edge* CampusMap(const Node* nodes, int nodeCount, int* edgeCount);

// Loads campus locations from data/locations.csv.
Node* loadCampusNodes(int* nodeCount);

// Frees the allocated arrays
void freeCampusData(Edge* edges, Node* nodes);

#endif // DATA_H
