#ifndef GRAPH_H
#define GRAPH_H
using namespace std;
#include "structure.h"
#include <vector>
#include <map>

class Graph {
private:
    
    map<int, vector<Edge>> adjList;     //so it takes Edge from stucture.h then create a map

public:
    
    void buildGraph(const vector<Edge>& rawEdges);

    // Returns all direct paths leading out of a specific location
    vector<Edge> getNeighbors(int nodeID);
};

#endif // GRAPH_H
