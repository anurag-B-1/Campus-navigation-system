#include "structure.h"
#include <vector>
using namespace std;

vector<Edge> CampusMap() {
    vector<Edge> edges;

    edges.push_back({1, 1, 2, 3, "Head East from Main Block towards CSE Block (approx. 50m)"});
    edges.push_back({2, 2, 1, 3, "Head West from CSE Block towards Main Block (approx. 50m)"});
    edges.push_back({3, 1, 3, 3, "Walk straight from Main Block to BTech 1 HOD / Cafeteria"});
    edges.push_back({4, 3, 1, 3, "Walk straight from BTech 1 HOD / Cafeteria to Main Block"});
    edges.push_back({5, 1, 4, 3, "Take the path from Main Block to Mechanical Block"});
    edges.push_back({6, 4, 1, 3, "Take the path from Mechanical Block to Main Block"});
    edges.push_back({7, 5, 6, 3, "Walk from Gate 1 to the Canteen"});
    edges.push_back({8, 6, 5, 3, "Walk from the Canteen to Gate 1"});
    edges.push_back({9, 5, 18, 3, "Walk from Gate 1 to Back Wing Parking"});
    edges.push_back({10, 18, 5, 3, "Walk from Back Wing Parking to Gate 1"});
    edges.push_back({11, 18, 19, 3, "Head towards the Civil Block from Back Wing Parking"});
    edges.push_back({12, 19, 18, 3, "Head towards Back Wing Parking from the Civil Block"});
    edges.push_back({13, 6, 8, 3, "Walk down the path from the Canteen to the Ground"});
    edges.push_back({14, 8, 6, 3, "Walk up the path from the Ground to the Canteen"});
    edges.push_back({15, 6, 9, 3, "Walk from the Canteen to the Sports Arena"});
    edges.push_back({16, 9, 6, 3, "Walk from the Sports Arena to the Canteen"});
    edges.push_back({17, 8, 1, 3, "Walk straight from the Ground to the Main Block"});
    edges.push_back({18, 1, 8, 3, "Walk straight from the Main Block to the Ground"});
    edges.push_back({19, 8, 7, 3, "Take the path from the Ground to the Workshop"});
    edges.push_back({20, 7, 8, 3, "Take the path from the Workshop to the Ground"});
    edges.push_back({21, 9, 10, 3, "Walk from the Sports Arena to the Basketball Court"});
    edges.push_back({22, 10, 9, 3, "Walk from the Basketball Court to the Sports Arena"});
    edges.push_back({23, 8, 9, 3, "Walk across from the Ground to the Sports Arena"});
    edges.push_back({24, 9, 8, 3, "Walk across from the Sports Arena to the Ground"});
    edges.push_back({25, 10, 11, 3, "Walk from the Basketball Court to the Volleyball Court"});
    edges.push_back({26, 11, 10, 3, "Walk from the Volleyball Court to the Basketball Court"});
    edges.push_back({27, 9, 12, 3, "Walk from the Sports Arena to Gate 2"});
    edges.push_back({28, 12, 9, 3, "Walk from Gate 2 to the Sports Arena"});
    edges.push_back({29, 11, 13, 3, "Head up to Tiger Hill from the Volleyball Court"});
    edges.push_back({30, 13, 11, 3, "Head down from Tiger Hill to the Volleyball Court"});
    edges.push_back({31, 10, 14, 3, "Walk from the Basketball Court to Gate 3"});
    edges.push_back({32, 14, 10, 3, "Walk from Gate 3 to the Basketball Court"});
    edges.push_back({33, 8, 14, 3, "Walk across from the Ground to Gate 3"});
    edges.push_back({34, 14, 8, 3, "Walk across from Gate 3 to the Ground"});
    edges.push_back({35, 14, 16, 3, "Walk from Gate 3 to the Agriculture Block"});
    edges.push_back({36, 16, 14, 3, "Walk from the Agriculture Block to Gate 3"});
    edges.push_back({37, 16, 15, 3, "Walk from the Agriculture Block to the Agri Canteen"});
    edges.push_back({38, 15, 16, 3, "Walk from the Agri Canteen to the Agriculture Block"});
    edges.push_back({39, 12, 17, 3, "Walk from Gate 2 to the Marina"});
    edges.push_back({40, 17, 12, 3, "Walk from the Marina to Gate 2"});

    // New edges for Central Library (Node 20)
    edges.push_back({41, 1, 20, 2, "Head East from Main Block towards the Central Library entrance walkway (approx. 70m)"});
    edges.push_back({42, 20, 1, 2, "Exit Central Library and head West along the corridor towards Main Block (approx. 70m)"});
    edges.push_back({43, 8, 20, 3, "From Ground, take the North-East paved pathway towards Central Library (approx. 120m)"});
    edges.push_back({44, 20, 8, 3, "From Central Library, take the South-West pathway leading down to the Ground (approx. 120m)"});
    edges.push_back({45, 2, 20, 2, "From CSE Block, cross the central courtyard towards Central Library (approx. 80m)"});
    edges.push_back({46, 20, 2, 2, "Exit Central Library and walk across the central courtyard to CSE Block (approx. 80m)"});

    return edges;
}

vector<Node> loadCampusNodes() {
    return {
        {1, "Main Block"},
        {2, "CSE Block"},
        {3, "BTech 1 HOD / Cafeteria"},
        {4, "Mechanical Block"},
        {5, "Gate 1"},
        {6, "Canteen"},
        {7, "Workshop"},
        {8, "Ground"},
        {9, "Sports Arena"},
        {10, "Basketball Court"},
        {11, "Volleyball Court"},
        {12, "Gate 2"},
        {13, "Tiger Hill"},
        {14, "Gate 3"},
        {15, "Agri Canteen"},
        {16, "Agriculture Block"},
        {17, "Marina"},
        {18, "Back Wing Parking"},
        {19, "Civil Block"},
        {20, "Central Library"}
    };
}
