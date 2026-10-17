#ifndef LINKED_LIST_H
#define LINKED_LIST_H


// 1. EdgeNode: Linked list node representing a pathway in an Adjacency List
struct EdgeNode {
  int edge_id;
  int from_node;
  int to_node;
  int weight;
  char description[128];
  EdgeNode *next; // Pointer to next edge

  EdgeNode() : edge_id(0), from_node(0), to_node(0), weight(0), next(nullptr) {
    description[0] = '\0';
  }

  EdgeNode(int id, int from, int to, int w, const char *desc = "")
      : edge_id(id), from_node(from), to_node(to), weight(w), next(nullptr) {
    if (desc) {
      int i = 0;
      while (desc[i] != '\0' && i < 127) {
        description[i] = desc[i];
        i++;
      }
      description[i] = '\0';
    } else {
      description[0] = '\0';
    }
  }
};

struct AdjList {
  EdgeNode *head;

  AdjList() : head(nullptr) {}
};

struct NodeLL {
  int node_id;
  char name[64];
  NodeLL *next;

  NodeLL() : node_id(0), next(nullptr) { name[0] = '\0'; }

  NodeLL(int id, const char *n) : node_id(id), next(nullptr) {
    if (n) {
      int i = 0;
      while (n[i] != '\0' && i < 63) {
        name[i] = n[i];
        i++;
      }
      name[i] = '\0';
    } else {
      name[0] = '\0';
    }
  }
};

// 4. PathNodeLL: Linked list node representing a single step in a traversed
// route
struct PathNodeLL {
  int node_id;
  char direction[128];
  PathNodeLL *next; // Pointer to next step in journey

  PathNodeLL() : node_id(0), next(nullptr) { direction[0] = '\0'; }

  PathNodeLL(int id, const char *dir = "") : node_id(id), next(nullptr) {
    if (dir) {
      int i = 0;
      while (dir[i] != '\0' && i < 127) {
        direction[i] = dir[i];
        i++;
      }
      direction[i] = '\0';
    } else {
      direction[0] = '\0';
    }
  }
};

// 5. PathResultLL: Linked list based path result without any STL vectors
struct PathResultLL {
  bool path_exists;
  int total_weight;
  PathNodeLL *head; // Head pointer to the path sequence linked list

  PathResultLL() : path_exists(false), total_weight(0), head(nullptr) {}
};

// Inserts an edge at the front of an adjacency list (O(1))
inline void addEdgeToAdjList(AdjList &list, int id, int from, int to,
                             int weight, const char *desc) {
  EdgeNode *newNode = new EdgeNode(id, from, to, weight, desc);
  newNode->next = list.head;
  list.head = newNode;
}

// Safely deallocates an entire EdgeNode linked list to prevent memory leaks
inline void freeEdgeList(EdgeNode *head) {
  while (head != nullptr) {
    EdgeNode *temp = head;
    head = head->next;
    delete temp;
  }
}

// Safely deallocates an entire PathNodeLL linked list
inline void freePathList(PathNodeLL *head) {
  while (head != nullptr) {
    PathNodeLL *temp = head;
    head = head->next;
    delete temp;
  }
}

#endif // LINKED_LIST_H
