#ifndef DATA_H
#define DATA_H

#include "structure.h"
#include <vector>

// Loads all defined edges representing campus paths and walkways
std::vector<Edge> CampusMap();

// Loads all defined campus landmark nodes and IDs
std::vector<Node> loadCampusNodes();

#endif // DATA_H
