/*
Compilation Commands:
    g++ sorce/main.cpp sorce/data.cpp sorce/graph.cpp sorce/router.cpp -o
campus_nav.exe

    g++ main.cpp data.cpp graph.cpp router.cpp -o campus_nav.exe
Execution:
    ./campus_nav.exe

*/

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "../include/data.h"
#include "../include/graph.h"
#include "../include/router.h"
#include "../include/structure.h"

using namespace std;

void printLocations(const vector<Node> &nodes) {

  printf("\n       AVAILABLE CAMPUS LOCATIONS        \n\n\n");

  for (size_t i = 0; i < nodes.size(); i++) {
    printf(" [%2d] %s\n", nodes[i].node_id, nodes[i].name.c_str());
  }
}

string toLower(const string &str) {
  string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return lowerStr;
}

int findLocationByNameOrId(const string &input, const vector<Node> &nodes) {

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
      for (const auto &node : nodes) {
        if (node.node_id == id)
          return id;
      }
    } catch (...) {
      return -1;
    }
  }

  // Search by name substring
  string lowerInput = toLower(input);
  for (const auto &node : nodes) {
    string lowerName = toLower(node.name);
    if (lowerName.find(lowerInput) != string::npos) {
      return node.node_id; // Return the first partial match
    }
  }

  return -1; // Not found
}

string getNodeName(int id, const vector<Node> &nodes) {
  for (const auto &node : nodes) {
    if (node.node_id == id)
      return node.name;
  }
  return "Unknown";
}

void processRoute(int start, int dest, const Graph &campusGraph,
                  const vector<Edge> &rawEdges,
                  const vector<Node> &campusNodes) {
  printf("\nCalculating absolute shortest route using Dijkstra aalgorithm\n");

  PathResult result = findShortestPath(start, dest, campusGraph, rawEdges);

  if (!result.path_exists) {
    printf("\nNo connecting path exists between these points.\n");
  } else {

    // Print Visual Breadcrumb Trail
    printf("Route: ");
    for (size_t i = 0; i < result.node_sequence.size(); i++) {
      printf("[%s]", getNodeName(result.node_sequence[i], campusNodes).c_str());
      if (i < result.node_sequence.size() - 1) {
        printf(" --");
      }
    }
    printf("\n\n");

    printf("Total Cost/Weight: %d\n", result.total_weight);
    printf("Turn-by-Turn Walking Directions:\n");

    printf("Start at %s\n", getNodeName(start, campusNodes).c_str());
    for (size_t i = 0; i < result.path_descriptions.size(); i++) {
      printf("Step %d: %s\n", (int)(i + 1),
             result.path_descriptions[i].c_str());
    }
    printf("Arive at %s\n", getNodeName(dest, campusNodes).c_str());
  }
}

void showDirectConnections(int locId, const Graph &graph,
                           const vector<Node> &campusNodes) {
  vector<Edge> neighbors = graph.getNeighbors(locId);
  string locName = getNodeName(locId, campusNodes);

  printf("\n=========================================\n");
  printf(" DIRECT PATHWAYS FROM: %s (Node %d)\n", locName.c_str(), locId);
  printf("=========================================\n");

  if (neighbors.empty()) {
    printf(" No outgoing pathways registered from this location.\n");
  } else {
    for (size_t i = 0; i < neighbors.size(); i++) {
      string destName = getNodeName(neighbors[i].to_node, campusNodes);
      printf(" [%zu] Destination: %s (Node %d) | Weight: %d\n", i + 1,
             destName.c_str(), neighbors[i].to_node, neighbors[i].weight);
      printf("     Path Info: %s\n", neighbors[i].description.c_str());
    }
  }
  printf("=========================================\n");
}

int main(int argc, char *argv[]) {
  // 1. Load campus data from data module
  vector<Edge> rawEdges = CampusMap();
  vector<Node> campusNodes = loadCampusNodes();

  // 2. Build graph module representation
  Graph campusGraph;
  campusGraph.buildGraph(rawEdges);

  // 3. Command-line arguments processing
  if (argc >= 3) {
    string startInput = argv[1];
    string destInput = argv[2];

    int start = findLocationByNameOrId(startInput, campusNodes);
    int dest = findLocationByNameOrId(destInput, campusNodes);

    if (start == -1) {
      printf("Error: Invalid starting location '%s'\n", startInput.c_str());
      return 1;
    }
    if (dest == -1) {
      printf("Error: Invalid destination location '%s'\n", destInput.c_str());
      return 1;
    }

    processRoute(start, dest, campusGraph, rawEdges, campusNodes);
    return 0; // Exit after processing CLI
  }

  int choice = 0;
  string inputStart, inputDest, inputLoc;

  while (true) {
    printf("\n=========================================\n");
    printf("     CAMPUS NAVIGATION SYSTEM   \n");

    printf(" 1. see Campus Locations\n");
    printf(" 2. Find Short path\n");
    printf(" 3. View Direct Pathways from Location\n");
    printf(" 4. Exit System\n");
    printf(" Enter choice 1 to 4 ");

    string choiceStr;
    if (!(cin >> choiceStr)) {
      break;
    }

    try {
      choice = stoi(choiceStr);
    } catch (...) {
      choice = 0;
    }

    if (choice == 4) {
      printf("\nThank you for using Campus Navigation. Goodbye!\n");
      break;
    }

    switch (choice) {
    case 1: {
      printLocations(campusNodes);
      break;
    }
    case 2: {
      printLocations(campusNodes);

      printf("\nEnter Starting Location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputStart);

      printf("Enter Destination Location (ID number or Name): ");
      getline(cin, inputDest);

      int start = findLocationByNameOrId(inputStart, campusNodes);
      int dest = findLocationByNameOrId(inputDest, campusNodes);

      if (start == -1) {
        printf("\nError: Invalid Starting Location!\n");
        break;
      }
      if (dest == -1) {
        printf("\nError: Invalid Destination Location!\n");
        break;
      }

      processRoute(start, dest, campusGraph, rawEdges, campusNodes);
      break;
    }
    case 3: {
      printLocations(campusNodes);
      printf("\nEnter Location (ID number or Name): ");
      cin >> ws;
      getline(cin, inputLoc);

      int locId = findLocationByNameOrId(inputLoc, campusNodes);
      if (locId == -1) {
        printf("\nError: Location not found!\n");
        break;
      }

      showDirectConnections(locId, campusGraph, campusNodes);
      break;
    }
    default:
      printf("Invalid option! Please pick between 1 and 4.\n");
    }
  }

  return 0;
}
