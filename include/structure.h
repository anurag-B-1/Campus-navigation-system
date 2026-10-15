#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

struct Edge {
    int edge_id;
    int from_node;
    int to_node;
    int weight;
    std::string description;

    // Constructors: supports default init, emplace_back, and push_back
    Edge() : edge_id(0), from_node(0), to_node(0), weight(0), description("") {}
    Edge(int id, int from, int to, int w, const std::string& desc)
        : edge_id(id), from_node(from), to_node(to), weight(w), description(desc) {}
};

struct Node {
    int node_id;
    std::string name; // e.g., "Main Block", "Canteen"

    Node() : node_id(0), name("") {}
    Node(int id, const std::string& n) : node_id(id), name(n) {}
};

struct PathResult {
    bool path_exists;                      // Set to false if start and destination are disconnected
    int total_weight;                      // Total distance or time cost of the journey
    std::vector<int> node_sequence;        // Ordered list of node IDs (e.g., [5, 6, 8])
    std::vector<std::string> path_descriptions; // Step-by-step text directions to display

    PathResult() : path_exists(false), total_weight(0) {}
};

#endif // STRUCTURES_H