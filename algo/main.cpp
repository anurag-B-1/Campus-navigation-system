/*
g++ main.cpp data.cpp graph.cpp router.c -o campus_nav
./campus_nav.exe
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "structure.h"
#include "graph.h"
#include "router.h"
using namespace std;

vector<Edge> CampusMap();
vector<Node> loadCampusNodes();

void printLocations(const vector<Node>& nodes) {
    printf("\n=========================================\n");
    printf("       AVAILABLE CAMPUS LOCATIONS        \n");
    printf("=========================================\n");
    for (size_t i = 0; i < nodes.size(); i++) {
        printf(" [%2d] %s\n", nodes[i].node_id, nodes[i].name.c_str());
    }
    printf("=========================================\n");
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

int findLocationByNameOrId(const string& input, const vector<Node>& nodes) {
    // Check if input is a pure number
    bool isNumber = true;
    for (char c : input) {
        if (!isdigit(c) && !isspace(c)) {
            isNumber = false;
            break;
        }
    }
    
    // Trim spaces if it's a number (basic sanity)
    string cleanInput = input;
    cleanInput.erase(remove_if(cleanInput.begin(), cleanInput.end(), ::isspace), cleanInput.end());
    
    if (isNumber && !cleanInput.empty()) {
        int id = stoi(cleanInput);
        for (const auto& node : nodes) {
            if (node.node_id == id) return id;
        }
    }
    
    // If not found by number or is a string, search by name substring
    string lowerInput = toLower(input);
    for (const auto& node : nodes) {
        string lowerName = toLower(node.name);
        if (lowerName.find(lowerInput) != string::npos) {
            return node.node_id; // Return the first partial match
        }
    }
    
    return -1; // Not found
}

string getNodeName(int id, const vector<Node>& nodes) {
    for (const auto& node : nodes) {
        if (node.node_id == id) return node.name;
    }
    return "Unknown";
}

void processRoute(int start, int dest, const vector<Edge>& rawEdges, const vector<Node>& campusNodes) {
    printf("\nCalculating absolute shortest route using Dijkstra's algorithm...\n");

    // Pass raw memory array pointer from vector data for C compatibility
    PathResult result = findShortestPath(start, dest, rawEdges.data(), rawEdges.size());

    if (!result.path_exists) {
        printf("\n❌ No connecting path exists between these points.\n");
    } else {
        printf("\n=========================================\n");
        printf("         ROUTE FOUND SUCCESSFULLY       \n");
        printf("=========================================\n");
        
        // Print Visual Breadcrumb Trail
        printf(" Route: ");
        for (size_t i = 0; i < result.node_sequence.size(); i++) {
            printf("[%s]", getNodeName(result.node_sequence[i], campusNodes).c_str());
            if (i < result.node_sequence.size() - 1) {
                printf(" ──► ");
            }
        }
        printf("\n\n");
        
        printf(" Total Cost/Weight: %d\n", result.total_weight);
        printf(" Turn-by-Turn Walking Directions:\n");
        
        printf("   📍 Start at %s\n", getNodeName(start, campusNodes).c_str());
        for (size_t i = 0; i < result.path_descriptions.size(); i++) {
            printf("   🚶 Step %d: %s\n", (int)(i + 1), result.path_descriptions[i].c_str());
        }
        printf("   🏁 Arrive at %s\n", getNodeName(dest, campusNodes).c_str());
        printf("=========================================\n");
    }
}

int main(int argc, char* argv[]) {
    vector<Edge> rawEdges = CampusMap();
    vector<Node> campusNodes = loadCampusNodes();

    Graph campusGraph;
    campusGraph.buildGraph(rawEdges);

    // Command-line arguments processing
    if (argc >= 3) {
        string startInput = argv[1];
        string destInput = argv[2];
        
        int start = findLocationByNameOrId(startInput, campusNodes);
        int dest = findLocationByNameOrId(destInput, campusNodes);
        
        if (start == -1) {
            printf("❌ Error: Invalid starting location '%s'.\n", startInput.c_str());
            return 1;
        }
        if (dest == -1) {
            printf("❌ Error: Invalid destination location '%s'.\n", destInput.c_str());
            return 1;
        }
        
        processRoute(start, dest, rawEdges, campusNodes);
        return 0; // Exit after processing CLI
    }

    int choice = 0;
    string inputStart, inputDest;

    while (true) {
        printf("\n=========================================\n");
        printf("     CAMPUS TERMINAL NAVIGATION SYSTEM   \n");
        printf("=========================================\n");
        printf(" 1. View Campus Locations\n");
        printf(" 2. Find Shortest Route\n");
        printf(" 3. Exit System\n");
        printf(" Enter choice (1-3): ");
        
        string choiceStr;
        if (!(cin >> choiceStr)) {
            break;
        }
        
        try {
            choice = stoi(choiceStr);
        } catch (...) {
            choice = 0;
        }

        if (choice == 3) {
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
                cin >> ws; // clear whitespace before reading line
                getline(cin, inputStart);
                
                printf("Enter Destination Location (ID number or Name): ");
                // No need for cin >> ws here because getline reads the rest of the line
                getline(cin, inputDest);

                int start = findLocationByNameOrId(inputStart, campusNodes);
                int dest = findLocationByNameOrId(inputDest, campusNodes);

                if (start == -1) {
                    printf("\n❌ Error: Invalid Starting Location!\n");
                    break;
                }
                if (dest == -1) {
                    printf("\n❌ Error: Invalid Destination Location!\n");
                    break;
                }

                processRoute(start, dest, rawEdges, campusNodes);
                break;
            }
            default:
                printf("Invalid option! Please pick between 1 and 3.\n");
        }
    }

    return 0;
}
