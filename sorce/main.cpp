/*
Compilation Commands:
    g++ sorce/main.cpp sorce/data.cpp sorce/graph.cpp sorce/router.cpp -o campus_nav.exe
Execution:
    ./campus_nav.exe
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdio>

#include "../include/structure.h"
#include "../include/data.h"
#include "../include/graph.h"
#include "../include/router.h"

using namespace std;

void printLocations(const Node* nodes, int nodeCount) {
    printf("\n       AVAILABLE CAMPUS LOCATIONS        \n\n\n");
    for (int i = 0; i < nodeCount; i++) {
        printf(" [%2d] %s\n", nodes[i].node_id, nodes[i].name);
    }
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
              [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

int findLocationByNameOrId(const string& input, const Node* nodes, int nodeCount) {
    bool isNumber = true;
    for (char c : input) {
        if (!isdigit(c) && !isspace(c)) {
            isNumber = false;
            break;
        }
    }

    string cleanInput = input;
    cleanInput.erase(remove_if(cleanInput.begin(), cleanInput.end(), ::isspace), cleanInput.end());

    if (isNumber && !cleanInput.empty()) {
        try {
            int id = stoi(cleanInput);
            for (int i = 0; i < nodeCount; i++) {
                if (nodes[i].node_id == id) return id;
            }
        } catch (...) {
            return -1;
        }
    }

    // Search by name substring
    string lowerInput = toLower(input);
    for (int i = 0; i < nodeCount; i++) {
        string lowerName = toLower(nodes[i].name);
        if (lowerName.find(lowerInput) != string::npos) {
            return nodes[i].node_id; // First partial match
        }
    }

    return -1; // Not found
}

const char* getNodeName(int id, const Node* nodes, int nodeCount) {
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i].node_id == id) return nodes[i].name;
    }
    return "Unknown";
}

void processRoute(int start, int dest, const Graph& campusGraph,
                  const Edge* rawEdges, int edgeCount,
                  const Node* campusNodes, int nodeCount) {
    printf("\nCalculating absolute shortest route using Dijkstra algorithm...\n");

    PathResult result = findShortestPath(start, dest, campusGraph, rawEdges, edgeCount);

    if (!result.path_exists) {
        printf("\nNo connecting path exists between these points.\n");
    } else {
        printf("\n=========================================\n");
        printf("         ROUTE FOUND SUCCESSFULLY        \n");
        printf("=========================================\n");

        // Print Visual Breadcrumb Trail using dynamic array
        printf(" Route: ");
        for (int i = 0; i < result.sequence_count; i++) {
            printf("[%s]", getNodeName(result.node_sequence[i], campusNodes, nodeCount));
            if (i < result.sequence_count - 1) {
                printf(" ──► ");
            }
        }
        printf("\n\n");

        printf(" Total Cost/Weight: %d\n", result.total_weight);
        printf(" Turn-by-Turn Walking Directions:\n");

        printf("   📍 Start at %s\n", getNodeName(start, campusNodes, nodeCount));
        for (int i = 0; i < result.description_count; i++) {
            printf("   🚶 Step %d: %s\n", i + 1, result.path_descriptions[i]);
        }
        printf("   🏁 Arrive at %s\n", getNodeName(dest, campusNodes, nodeCount));
        printf("=========================================\n");
    }

    // Safely free memory allocated for PathResult dynamic arrays
    freePathResult(result);
}

void showDirectConnections(int locId, const Graph& graph,
                           const Node* campusNodes, int nodeCount) {
    const char* locName = getNodeName(locId, campusNodes, nodeCount);

    printf("\n=========================================\n");
    printf(" DIRECT PATHWAYS FROM: %s (Node %d)\n", locName, locId);
    printf("=========================================\n");

    EdgeNode* edge = graph.getNeighbors(locId);
    if (!edge) {
        printf(" No outgoing pathways registered from this location.\n");
    } else {
        int idx = 1;
        while (edge != nullptr) {
            const char* destName = getNodeName(edge->to_node, campusNodes, nodeCount);
            printf(" [%d] Destination: %s (Node %d) | Weight: %d\n",
                   idx, destName, edge->to_node, edge->weight);
            printf("     Path Info: %s\n", edge->description);
            edge = edge->next;
            idx++;
        }
    }
    printf("=========================================\n");
}

int main(int argc, char* argv[]) {
    // 1. Load campus data into dynamic arrays allocated via calloc/malloc
    int edgeCount = 0;
    int nodeCount = 0;
    Edge* rawEdges = CampusMap(&edgeCount);
    Node* campusNodes = loadCampusNodes(&nodeCount);

    // 2. Build graph using C-style array of linked lists
    Graph campusGraph;
    campusGraph.buildGraph(rawEdges, edgeCount);

    // 3. Command-line arguments processing
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

        processRoute(start, dest, campusGraph, rawEdges, edgeCount, campusNodes, nodeCount);
        freeCampusData(rawEdges, campusNodes);
        return 0;
    }

    int choice = 0;
    string inputStart, inputDest, inputLoc;

    while (true) {
        printf("\n=========================================\n");
        printf("     CAMPUS NAVIGATION SYSTEM (DSA IN C) \n");
        printf("=========================================\n");
        printf(" 1. View Campus Locations\n");
        printf(" 2. Find Shortest Route\n");
        printf(" 3. View Direct Pathways from Location\n");
        printf(" 4. Exit System\n");
        printf(" Enter choice (1-4): ");

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

                processRoute(start, dest, campusGraph, rawEdges, edgeCount, campusNodes, nodeCount);
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
            default:
                printf("Invalid option! Please pick between 1 and 4.\n");
        }
    }

    // Clean up heap allocated dynamic arrays before exiting
    freeCampusData(rawEdges, campusNodes);

    return 0;
}
