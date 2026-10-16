#ifndef DATA_H
#define DATA_H

#include "structure.h"

// Returns dynamic array allocated with malloc/calloc
Edge* CampusMap(int* edgeCount);

// Returns dynamic array of campus nodes allocated with malloc/calloc
Node* loadCampusNodes(int* nodeCount);

// Frees the allocated arrays
void freeCampusData(Edge* edges, Node* nodes);

#endif // DATA_H
