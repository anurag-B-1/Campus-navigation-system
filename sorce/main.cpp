
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/data.h"
#include "../include/graph.h"
#include "../include/router.h"
#include "../include/search.h"
#include "../include/structure.h"

using namespace std;

const char *getNodeName(int id, const Node *nodes, int nodeCount);

const char *RECENT_ROUTES_FILE = "recent_routes.txt";

void saveRecentRoute(int start, int dest, const char *algorithm) {
  vector<string> recent;
  ifstream input(RECENT_ROUTES_FILE);
  string line;
  while (getline(input, line)) {
    if (!line.empty()) recent.push_back(line);
  }
  input.close();

  recent.insert(recent.begin(), to_string(start) + " " + to_string(dest) + " " + algorithm);
  if (recent.size() > 10) recent.resize(10);

  ofstream output(RECENT_ROUTES_FILE, ios::trunc);
  for (const string &entry : recent) output << entry << '\n';
}

void showRecentRoutes(const Node *nodes, int nodeCount) {
  ifstream input(RECENT_ROUTES_FILE);
  if (!input) {
    printf("\nNo saved route searches yet.\n");
    return;
  }
  printf("\n RECENT ROUTE SEARCHES (latest 10)\n\n");
  int start, dest;
  string algorithm;
  int index = 1;
  while (input >> start >> dest >> algorithm) {
    printf(" %d. %s: %s -> %s\n", index++, algorithm.c_str(),
           getNodeName(start, nodes, nodeCount), getNodeName(dest, nodes, nodeCount));
  }
  if (index == 1) printf(" No saved route searches yet.\n");
}

void printLocations(const Node *nodes, int nodeCount) {
  printf("\n       AVAILABLE CAMPUS LOCATIONS        \n\n\n");
  for (int i = 0; i < nodeCount; i++) {
    printf(" [%2d] %s\n", nodes[i].node_id, nodes[i].name);
  }
}

  string toLower(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
              [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

int findLocationByNameOrId(const string &input, const Node *nodes,
                           int nodeCount) {
  bool isNumber = true;
  for (char c : input) {
    if (!isdigit(c) && !isspace(c)) {
      isNumber = false;
      break;
    }
  }

  string cleanInput = input;
  cleanInput.erase(remove_if(cleanInput.begin(), cleanInput.end(), ::isspace),
                   cleanInput.end());

  if (isNumber && !cleanInput.empty()) {
    try {
      int id = stoi(cleanInput);
      for (int i = 0; i < nodeCount; i++) {
        if (nodes[i].node_id == id)
          return id;
      }
    } catch (...) {
      return -1;
    }
  }

  string lowerInput = toLower(input);
  for (int i = 0; i < nodeCount; i++) {
    string lowerName = toLower(nodes[i].name);
    string lowerLocationId = toLower(nodes[i].location_id);
    if (lowerName.find(lowerInput) != string::npos ||
        lowerLocationId == lowerInput) {
      return nodes[i].node_id; 
    }
  }

  return -1; 
}

const char *getNodeName(int id, const Node *nodes, int nodeCount) {
  for (int i = 0; i < nodeCount; i++) {
    if (nodes[i].node_id == id)
      return nodes[i].name;
  }
  return "Unknown";
}

void processRoute(int start, int dest, const Graph &campusGraph,
                  const Edge *rawEdges, int edgeCount, const Node *campusNodes,
                  int nodeCount) {
  printf("\nCalculating absolute shortest route using Dijkstra algorithm...\n");

  PathResult result =
      findShortestPath(start, dest, campusGraph, rawEdges, edgeCount);

  if (!result.path_exists) {
    printf("\nNo connecting path exists between these points.\n");
  } else {
    saveRecentRoute(start, dest, "Dijkstra");
    printf("\n\nroot founted \n\n\n");

    // Print Visual Breadcrumb Trail using dynamic array
    printf(" Route: ");
    for (int i = 0; i < result.sequence_count; i++) {
      printf("[%s]",
             getNodeName(result.node_sequence[i], campusNodes, nodeCount));
      if (i < result.sequence_count - 1) {
        printf(" ──► ");
      }
    }
    printf("\n\n");

    printf(" Total Distance: %.1f m\n", result.total_weight);
    printf(" Turn-by-Turn Walking Directions:\n");

    printf("Start at %s\n", getNodeName(start, campusNodes, nodeCount));
    for (int i = 0; i < result.description_count; i++) {
      printf(" Step %d: %s\n", i + 1, result.path_descriptions[i]);
    }
    printf("   Arrive at %s\n", getNodeName(dest, campusNodes, nodeCount));
    printf("=========================================\n");
  }

  // Safely free memory allocated for PathResult dynamic arrays
  freePathResult(result);
}

void printNodeIdList(const IntList &list, const Node *campusNodes,
                     int nodeCount) {
  if (list.count <= 0 || !list.items) {
    printf(" (none)\n");
    return;
  }
  for (int i = 0; i < list.count; i++) {
    printf(" [%d] %s (Node %d)\n", i + 1,
           getNodeName(list.items[i], campusNodes, nodeCount), list.items[i]);
  }
}

void showDirectConnections(int locId, const Graph &graph,
                           const Node *campusNodes, int nodeCount) {
  const char *locName = getNodeName(locId, campusNodes, nodeCount);


  printf("\n DIRECT PATHWAYS FROM: %s (Node %d)\n\n", locName, locId);


  EdgeNode *edge = graph.getNeighbors(locId);
  if (!edge) {
    printf(" No outgoing pathways registered from this location.\n");
  } else {
    int idx = 1;
    while (edge != nullptr) {
      const char *destName = getNodeName(edge->to_node, campusNodes, nodeCount);
      printf(" [%d] Destination: %s (Node %d) | Distance: %.1f m\n", idx, destName,
             edge->to_node, edge->weight);
      printf("     Path Info: %s\n", edge->description);
      edge = edge->next;
      idx++;
    }
  }
  printf("=========================================\n");
}

int main(int argc, char *argv[]) {
  
  int edgeCount = 0;
  int nodeCount = 0;
  Node *campusNodes = loadCampusNodes(&nodeCount);
  Edge *rawEdges = CampusMap(campusNodes, nodeCount, &edgeCount);

  if (!campusNodes || !rawEdges || nodeCount == 0 || edgeCount == 0) {
    fprintf(stderr, "Unable to load campus data. Check data/locations.csv and data/connections.csv.\n");
    freeCampusData(rawEdges, campusNodes);
    return 1;
  }

  Graph campusGraph;
  campusGraph.buildGraph(rawEdges, edgeCount);

  if (argc >= 3) {
    string startInput = argv[1];
    string destInput = argv[2];

    int start = findLocationByNameOrId(startInput, campusNodes, nodeCount);
    int dest = findLocationByNameOrId(destInput, campusNodes, nodeCount);

    if (start == -1) {
      printf("Error: Invalid starting location '%s'\n", startInput.c_str());
      freeCampusData(rawEdges, campusNodes);
      return 1;
    }
    if (dest == -1) {
      printf("Error: Invalid destination location '%s'\n", destInput.c_str());
      freeCampusData(rawEdges, campusNodes);
      return 1;
    }

    processRoute(start, dest, campusGraph, rawEdges, edgeCount, campusNodes,
                 nodeCount);
    freeCampusData(rawEdges, campusNodes);
    return 0;
  }

  int choice = 0;
  string inputStart, inputDest, inputLoc;

  while (true) {
    printf("=========================\n\n");
    printf("1. View Campus Locations\n");
    printf("2. Find Shortest Route\n");
    printf("3. View Direct Pathways from Location\n");
    printf("4. Search Rooms / Locations\n");
    printf("5. Find Fewest-Hop Route (BFS)\n");
    printf("6. Explore Locations (DFS)\n");
    printf("7. View Recent Route Searches\n");
    printf("8. Exit System\n");
    printf("choose 1-8: ");

    string choiceStr;
    if (!(cin >> choiceStr)) {
      break;
    }

    try {
      choice = stoi(choiceStr);
    } catch (...) {
      choice = 0;
    }

    if (choice == 8) {
      printf("\nThank you for using Campus Navigation. Goodbye!\n");
      break;
    }

    switch (choice) {
    case 1: {
      printLocations(campusNodes, nodeCount);
      break;
    }
    case 2: {
      printLocations(campusNodes, nodeCount);

      printf("\nEnter Starting Location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputStart);

      printf("Enter Destination Location (ID number or Name): ");
      getline(cin, inputDest);

      int start = findLocationByNameOrId(inputStart, campusNodes, nodeCount);
      int dest = findLocationByNameOrId(inputDest, campusNodes, nodeCount);

      if (start == -1) {
        printf("\nError: Invalid Starting Location!\n");
        break;
      }
      if (dest == -1) {
        printf("\nError: Invalid Destination Location!\n");
        break;
      }

      processRoute(start, dest, campusGraph, rawEdges, edgeCount, campusNodes,
                   nodeCount);
      break;
    }
    case 3: {
      printLocations(campusNodes, nodeCount);
      printf("\nEnter Location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputLoc);

      int locId = findLocationByNameOrId(inputLoc, campusNodes, nodeCount);
      if (locId == -1) {
        printf("\nError: Location not found!\n");
        break;
      }

      showDirectConnections(locId, campusGraph, campusNodes, nodeCount);
      break;
    }
    case 4: {
      printf("\nEnter room or location name (partial match allowed): ");
      cin >> ws;
      getline(cin, inputLoc);

      IntList matches =
          searchLocations(campusNodes, nodeCount, inputLoc.c_str());
      if (matches.count == 0) {
        printf("\nNo rooms or locations matched '%s'.\n", inputLoc.c_str());
      } else {
        printf("\n Search results for '%s':\n", inputLoc.c_str());
        printNodeIdList(matches, campusNodes, nodeCount);
      }
      freeIntList(&matches);
      break;
    }
    case 5: {
      printLocations(campusNodes, nodeCount);
      printf("\nEnter Starting Location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputStart);
      printf("Enter Destination Location (ID number or Name): ");
      getline(cin, inputDest);
      int start = findLocationByNameOrId(inputStart, campusNodes, nodeCount);
      int dest = findLocationByNameOrId(inputDest, campusNodes, nodeCount);
      if (start == -1 || dest == -1) {
        printf("\nError: Invalid starting location or destination.\n");
        break;
      }
      IntList path = findBfsPath(campusGraph, start, dest);
      if (path.count == 0) {
        printf("\nNo connecting path exists between these locations.\n");
      } else {
        printf("\nFewest-hop BFS route (%d hops): ", path.count - 1);
        for (int i = 0; i < path.count; ++i) {
          printf("%s%s", getNodeName(path.items[i], campusNodes, nodeCount),
                 i + 1 < path.count ? " -> " : "\n");
        }
        saveRecentRoute(start, dest, "BFS");
      }
      freeIntList(&path);
      break;
    }
    case 6: {
      printLocations(campusNodes, nodeCount);
      printf("\nEnter starting location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputLoc);
      int start = findLocationByNameOrId(inputLoc, campusNodes, nodeCount);
      if (start == -1) {
        printf("\nError: Location not found.\n");
        break;
      }
      TraversalResult order = dfsTraversal(campusGraph, start);
      printf("\nDFS exploration order:\n");
      for (int i = 0; i < order.count; ++i)
        printf(" %d. %s (depth %d)\n", i + 1,
               getNodeName(order.items[i], campusNodes, nodeCount), order.hops[i]);
      freeTraversalResult(&order);
      break;
    }
    case 7:
      showRecentRoutes(campusNodes, nodeCount);
      break;

    default:
      printf("Invalid option! Please pick between 1 and 8.\n");
    }
  }

  // Clean up heap allocated dynamic arrays before exiting
  freeCampusData(rawEdges, campusNodes);
  return 0;
}
