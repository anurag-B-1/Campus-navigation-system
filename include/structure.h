#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <cstdlib>
#include <cstring>

struct Edge {
  int edge_id;
  int from_node;
  int to_node;
  int weight;
  char description[128];

  Edge() : edge_id(0), from_node(0), to_node(0), weight(0) {
    description[0] = '\0';
  }

  Edge(int id, int from, int to, int w, const char *desc = "")
      : edge_id(id), from_node(from), to_node(to), weight(w) {
    if (desc) {
      std::strncpy(description, desc, sizeof(description) - 1);
      description[sizeof(description) - 1] = '\0';
    } else {
      description[0] = '\0';
    }
  }
};

struct Node {
  int node_id;
  char name[64];

  Node() : node_id(0) { name[0] = '\0'; }

  Node(int id, const char *n = "") : node_id(id) {
    if (n) {
      std::strncpy(name, n, sizeof(name) - 1);
      name[sizeof(name) - 1] = '\0';
    } else {
      name[0] = '\0';
    }
  }
};

struct PathResult {
  bool path_exists;
  int total_weight;
  int *node_sequence; // Dynamic array allocated via malloc/calloc
  int sequence_count;
  char **path_descriptions; // Dynamic array of string pointers allocated via
                            // malloc/calloc
  int description_count;

  PathResult()
      : path_exists(false), total_weight(0), node_sequence(nullptr),
        sequence_count(0), path_descriptions(nullptr), description_count(0) {}
};

// Safe memory deallocation for PathResult dynamic arrays
inline void freePathResult(PathResult &res) {
  if (res.node_sequence) {
    std::free(res.node_sequence);
    res.node_sequence = nullptr;
  }
  if (res.path_descriptions) {
    for (int i = 0; i < res.description_count; i++) {
      if (res.path_descriptions[i]) {
        std::free(res.path_descriptions[i]);
      }
    }
    std::free(res.path_descriptions);
    res.path_descriptions = nullptr;
  }
  res.sequence_count = 0;
  res.description_count = 0;
}

#include "linked_list.h"

#endif // STRUCTURES_H