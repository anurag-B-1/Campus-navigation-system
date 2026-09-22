#ifndef STRUCTURES_H
#define STRUCTURES_H
using namespace std;
#include <string>
#include <vector>

struct Edge {
    int edge_id;
    int from_node;
    int to_node;
    int weight;
    string description;
};

struct Node {
    int node_id;
    string name; // e.g., "Main Block", "Canteen"
};

struct PathResult {
    bool path_exists;              // Set to false if start and destination are disconnected
    int total_weight;              // Total distance or time cost of the journey
    vector<int> node_sequence; // Ordered list of node IDs (e.g., [5, 6, 8] for Gate 1 -> Canteen -> Ground)
    vector<std::string> path_descriptions; // Step-by-step text directions to display
};

#endif // STRUCTURES_H


/*
#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
#include <vector>

using namespace std;

class Edge {
private:
    int edge_id;
    int from_node;
    int to_node;
    int weight;
    string description;

public:
    // Constructors
    Edge() : edge_id(0), from_node(0), to_node(0), weight(0), description("") {}
    Edge(int id, int from, int to, int w, string desc) 
        : edge_id(id), from_node(from), to_node(to), weight(w), description(desc) {}

    // Getters
    int getEdgeId() const { return edge_id; }
    int getFromNode() const { return from_node; }
    int getToNode() const { return to_node; }
    int getWeight() const { return weight; }
    string getDescription() const { return description; }
};

class Node {
private:
    int node_id;
    string name;

public:
    // Constructors
    Node() : node_id(0), name("") {}
    Node(int id, string n) : node_id(id), name(n) {}

    // Getters
    int getNodeId() const { return node_id; }
    string getName() const { return name; }
};

class PathResult {
private:
    bool path_exists;
    int total_weight;
    vector<int> node_sequence;
    vector<string> path_descriptions;

public:
    // Constructor
    PathResult() : path_exists(false), total_weight(0) {}

    // Getters
    bool getPathExists() const { return path_exists; }
    int getTotalWeight() const { return total_weight; }
    const vector<int>& getNodeSequence() const { return node_sequence; }
    const vector<string>& getPathDescriptions() const { return path_descriptions; }

    // Setters
    void setPathExists(bool status) { path_exists = status; }
    void setTotalWeight(int weight) { total_weight = weight; }
    void addNodeToSequence(int node) { node_sequence.push_back(node); }
    void addPathDescription(const string& desc) { path_descriptions.push_back(desc); }
};

#endif // STRUCTURES_H
*/