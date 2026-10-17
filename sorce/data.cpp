#include "../include/data.h"
#include <cstdlib>
#include <cstring>

Edge *CampusMap(int *edgeCount) {
  const int TOTAL_EDGES = 46;
  if (edgeCount)
    *edgeCount = TOTAL_EDGES;

  // Allocate dynamic array using calloc (DSA in C)
  Edge *edges = (Edge *)std::calloc(TOTAL_EDGES, sizeof(Edge));
  if (!edges)
    return nullptr;

  auto setEdge = [&](int idx, int id, int from, int to, int w,
                     const char *desc) {
    edges[idx].edge_id = id;
    edges[idx].from_node = from;
    edges[idx].to_node = to;
    edges[idx].weight = w;
    std::strncpy(edges[idx].description, desc,
                 sizeof(edges[idx].description) - 1);
    edges[idx].description[sizeof(edges[idx].description) - 1] = '\0';
  };

  setEdge(0, 1, 1, 2, 3,
          "Head East from Main Block towards CSE Block (approx. 50m)");
  setEdge(1, 2, 2, 1, 3,
          "Head West from CSE Block towards Main Block (approx. 50m)");
  setEdge(2, 3, 1, 3, 3,
          "Walk straight from Main Block to BTech 1 HOD / Cafeteria");
  setEdge(3, 4, 3, 1, 3,
          "Walk straight from BTech 1 HOD / Cafeteria to Main Block");
  setEdge(4, 5, 1, 4, 3, "Take the path from Main Block to Mechanical Block");
  setEdge(5, 6, 4, 1, 3, "Take the path from Mechanical Block to Main Block");
  setEdge(6, 7, 5, 6, 3, "Walk from Gate 1 to the Canteen");
  setEdge(7, 8, 6, 5, 3, "Walk from the Canteen to Gate 1");
  setEdge(8, 9, 5, 18, 3, "Walk from Gate 1 to Back Wing Parking");
  setEdge(9, 10, 18, 5, 3, "Walk from Back Wing Parking to Gate 1");
  setEdge(10, 11, 18, 19, 3,
          "Head towards the Civil Block from Back Wing Parking");
  setEdge(11, 12, 19, 18, 3,
          "Head towards Back Wing Parking from the Civil Block");
  setEdge(12, 13, 6, 8, 3, "Walk down the path from the Canteen to the Ground");
  setEdge(13, 14, 8, 6, 3, "Walk up the path from the Ground to the Canteen");
  setEdge(14, 15, 6, 9, 3, "Walk from the Canteen to the Sports Arena");
  setEdge(15, 16, 9, 6, 3, "Walk from the Sports Arena to the Canteen");
  setEdge(16, 17, 8, 1, 3, "Walk straight from the Ground to the Main Block");
  setEdge(17, 18, 1, 8, 3, "Walk straight from the Main Block to the Ground");
  setEdge(18, 19, 8, 7, 3, "Take the path from the Ground to the Workshop");
  setEdge(19, 20, 7, 8, 3, "Take the path from the Workshop to the Ground");
  setEdge(20, 21, 9, 10, 3,
          "Walk from the Sports Arena to the Basketball Court");
  setEdge(21, 22, 10, 9, 3,
          "Walk from the Basketball Court to the Sports Arena");
  setEdge(22, 23, 8, 9, 3, "Walk across from the Ground to the Sports Arena");
  setEdge(23, 24, 9, 8, 3, "Walk across from the Sports Arena to the Ground");
  setEdge(24, 25, 10, 11, 3,
          "Walk from the Basketball Court to the Volleyball Court");
  setEdge(25, 26, 11, 10, 3,
          "Walk from the Volleyball Court to the Basketball Court");
  setEdge(26, 27, 9, 12, 3, "Walk from the Sports Arena to Gate 2");
  setEdge(27, 28, 12, 9, 3, "Walk from Gate 2 to the Sports Arena");
  setEdge(28, 29, 11, 13, 3, "Head up to Tiger Hill from the Volleyball Court");
  setEdge(29, 30, 13, 11, 3,
          "Head down from Tiger Hill to the Volleyball Court");
  setEdge(30, 31, 10, 14, 3, "Walk from the Basketball Court to Gate 3");
  setEdge(31, 32, 14, 10, 3, "Walk from Gate 3 to the Basketball Court");
  setEdge(32, 33, 8, 14, 3, "Walk across from the Ground to Gate 3");
  setEdge(33, 34, 14, 8, 3, "Walk across from Gate 3 to the Ground");
  setEdge(34, 35, 14, 16, 3, "Walk from Gate 3 to the Agriculture Block");
  setEdge(35, 36, 16, 14, 3, "Walk from the Agriculture Block to Gate 3");
  setEdge(36, 37, 16, 15, 3,
          "Walk from the Agriculture Block to the Agri Canteen");
  setEdge(37, 38, 15, 16, 3,
          "Walk from the Agri Canteen to the Agriculture Block");
  setEdge(38, 39, 12, 17, 3, "Walk from Gate 2 to the Marina");
  setEdge(39, 40, 17, 12, 3, "Walk from the Marina to Gate 2");

  // Edges for Central Library (Node 20)
  setEdge(40, 41, 1, 20, 2,
          "Head East from Main Block towards the Central Library entrance "
          "walkway (approx. 70m)");
  setEdge(41, 42, 20, 1, 2,
          "Exit Central Library and head West along the corridor towards Main "
          "Block (approx. 70m)");
  setEdge(42, 43, 8, 20, 3,
          "From Ground, take the North-East paved pathway towards Central "
          "Library (approx. 120m)");
  setEdge(43, 44, 20, 8, 3,
          "From Central Library, take the South-West pathway leading down to "
          "the Ground (approx. 120m)");
  setEdge(44, 45, 2, 20, 2,
          "From CSE Block, cross the central courtyard towards Central Library "
          "(approx. 80m)");
  setEdge(45, 46, 20, 2, 2,
          "Exit Central Library and walk across the central courtyard to CSE "
          "Block (approx. 80m)");

  return edges;
}

Node *loadCampusNodes(int *nodeCount) {
  const int TOTAL_NODES = 20;
  if (nodeCount)
    *nodeCount = TOTAL_NODES;

  // Allocate dynamic array using calloc (DSA in C)
  Node *nodes = (Node *)std::calloc(TOTAL_NODES, sizeof(Node));
  if (!nodes)
    return nullptr;

  auto setNode = [&](int idx, int id, const char *name) {
    nodes[idx].node_id = id;
    std::strncpy(nodes[idx].name, name, sizeof(nodes[idx].name) - 1);
    nodes[idx].name[sizeof(nodes[idx].name) - 1] = '\0';
  };

  setNode(0, 1, "Main Block");
  setNode(1, 2, "CSE Block");
  setNode(2, 3, "BTech 1 HOD / Cafeteria");
  setNode(3, 4, "Mechanical Block");
  setNode(4, 5, "Gate 1");
  setNode(5, 6, "Canteen");
  setNode(6, 7, "Workshop");
  setNode(7, 8, "Ground");
  setNode(8, 9, "Sports Arena");
  setNode(9, 10, "Basketball Court");
  setNode(10, 11, "Volleyball Court");
  setNode(11, 12, "Gate 2");
  setNode(12, 13, "Tiger Hill");
  setNode(13, 14, "Gate 3");
  setNode(14, 15, "Agri Canteen");
  setNode(15, 16, "Agriculture Block");
  setNode(16, 17, "Marina");
  setNode(17, 18, "Back Wing Parking");
  setNode(18, 19, "Civil Block");
  setNode(19, 20, "Central Library");

  return nodes;
}

void freeCampusData(Edge *edges, Node *nodes) {
  if (edges)
    std::free(edges);
  if (nodes)
    std::free(nodes);
}
