#include "../include/data.h"
#include "../include/graph.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

const char* LOCATIONS_FILE = "data/locations.csv";
const char* CONNECTIONS_FILE = "data/connections.csv";

std::string trim(const std::string& value) {
  const std::string whitespace = " \t\r\n";
  const std::size_t first = value.find_first_not_of(whitespace);
  if (first == std::string::npos) return "";
  return value.substr(first, value.find_last_not_of(whitespace) - first + 1);
}

// Handles ordinary CSV fields and quoted fields containing commas or quotes.
std::vector<std::string> splitCsvLine(const std::string& line) {
  std::vector<std::string> fields;
  std::string field;
  bool inQuotes = false;

  for (std::size_t i = 0; i < line.size(); ++i) {
    const char current = line[i];
    if (current == '"') {
      if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
        field += '"';
        ++i;
      } else {
        inQuotes = !inQuotes;
      }
    } else if (current == ',' && !inQuotes) {
      fields.push_back(trim(field));
      field.clear();
    } else {
      field += current;
    }
  }

  fields.push_back(trim(field));
  return fields;
}

bool readHeader(std::ifstream& file, const std::vector<std::string>& expected,
                const char* fileName) {
  std::string line;
  if (!std::getline(file, line)) {
    std::fprintf(stderr, "Data file is empty: %s\n", fileName);
    return false;
  }

  // Remove a UTF-8 BOM if a spreadsheet editor wrote one.
  if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
      static_cast<unsigned char>(line[1]) == 0xBB &&
      static_cast<unsigned char>(line[2]) == 0xBF) {
    line.erase(0, 3);
  }

  if (splitCsvLine(line) != expected) {
    std::fprintf(stderr, "Unexpected header in %s\n", fileName);
    return false;
  }
  return true;
}

bool parsePositiveInt(const std::string& text, int& value) {
  try {
    std::size_t parsed = 0;
    const long long number = std::stoll(text, &parsed);
    if (parsed != text.size() || number <= 0 || number >= MAX_GRAPH_NODES) {
      return false;
    }
    value = static_cast<int>(number);
    return true;
  } catch (...) {
    return false;
  }
}

bool parsePositiveDistance(const std::string& text, double& value) {
  try {
    std::size_t parsed = 0;
    const double distance = std::stod(text, &parsed);
    if (parsed != text.size() || !std::isfinite(distance) || distance <= 0) {
      return false;
    }
    value = distance;
    return true;
  } catch (...) {
    return false;
  }
}

}  // namespace

Node* loadCampusNodes(int* nodeCount) {
  if (nodeCount) *nodeCount = 0;

  std::ifstream file(LOCATIONS_FILE);
  if (!file) {
    std::fprintf(stderr, "Could not open %s\n", LOCATIONS_FILE);
    return nullptr;
  }
  if (!readHeader(file, {"node_id", "location_id", "type", "name", "floor",
                         "description"}, LOCATIONS_FILE)) {
    return nullptr;
  }

  std::vector<Node> loadedNodes;
  std::unordered_map<int, bool> usedNodeIds;
  std::unordered_map<std::string, bool> usedLocationIds;
  std::string line;
  int lineNumber = 1;

  while (std::getline(file, line)) {
    ++lineNumber;
    if (trim(line).empty()) continue;
    const std::vector<std::string> fields = splitCsvLine(line);
    if (fields.size() != 6) {
      std::fprintf(stderr, "Invalid location row %d in %s\n", lineNumber,
                   LOCATIONS_FILE);
      return nullptr;
    }

    int nodeId = 0;
    if (!parsePositiveInt(fields[0], nodeId) || fields[1].empty() ||
        fields[3].empty() || usedNodeIds[nodeId] || usedLocationIds[fields[1]]) {
      std::fprintf(stderr, "Invalid or duplicate location row %d in %s\n",
                   lineNumber, LOCATIONS_FILE);
      return nullptr;
    }

    std::string displayName = fields[3];
    for (char& character : displayName) {
      if (character == '_') character = ' ';
    }

    usedNodeIds[nodeId] = true;
    usedLocationIds[fields[1]] = true;
    loadedNodes.push_back(Node(nodeId, fields[1].c_str(), displayName.c_str()));
  }

  if (loadedNodes.empty()) {
    std::fprintf(stderr, "No locations were found in %s\n", LOCATIONS_FILE);
    return nullptr;
  }

  Node* nodes = new Node[loadedNodes.size()];
  for (std::size_t i = 0; i < loadedNodes.size(); ++i) nodes[i] = loadedNodes[i];
  if (nodeCount) *nodeCount = static_cast<int>(loadedNodes.size());
  return nodes;
}

Edge* CampusMap(const Node* nodes, int nodeCount, int* edgeCount) {
  if (edgeCount) *edgeCount = 0;
  if (!nodes || nodeCount <= 0) return nullptr;

  std::unordered_map<std::string, int> locationToNode;
  for (int i = 0; i < nodeCount; ++i) {
    locationToNode[nodes[i].location_id] = nodes[i].node_id;
  }

  std::ifstream file(CONNECTIONS_FILE);
  if (!file) {
    std::fprintf(stderr, "Could not open %s\n", CONNECTIONS_FILE);
    return nullptr;
  }
  if (!readHeader(file, {"from", "to", "distance"}, CONNECTIONS_FILE)) {
    return nullptr;
  }

  std::vector<Edge> loadedEdges;
  std::string line;
  int lineNumber = 1;
  while (std::getline(file, line)) {
    ++lineNumber;
    if (trim(line).empty()) continue;
    const std::vector<std::string> fields = splitCsvLine(line);
    if (fields.size() != 3) {
      std::fprintf(stderr, "Invalid connection row %d in %s\n", lineNumber,
                   CONNECTIONS_FILE);
      return nullptr;
    }

    const auto from = locationToNode.find(fields[0]);
    const auto to = locationToNode.find(fields[1]);
    double distance = 0;
    if (from == locationToNode.end() || to == locationToNode.end() ||
        !parsePositiveDistance(fields[2], distance)) {
      std::fprintf(stderr, "Invalid connection row %d in %s\n", lineNumber,
                   CONNECTIONS_FILE);
      return nullptr;
    }

    char description[128];
    std::snprintf(description, sizeof(description), "Walk from %s to %s (%.1f m)",
                  fields[0].c_str(), fields[1].c_str(), distance);
    loadedEdges.push_back(Edge(static_cast<int>(loadedEdges.size() + 1),
                               from->second, to->second, distance, description));
  }

  if (loadedEdges.empty()) {
    std::fprintf(stderr, "No connections were found in %s\n", CONNECTIONS_FILE);
    return nullptr;
  }

  Edge* edges = new Edge[loadedEdges.size()];
  for (std::size_t i = 0; i < loadedEdges.size(); ++i) edges[i] = loadedEdges[i];
  if (edgeCount) *edgeCount = static_cast<int>(loadedEdges.size());
  return edges;
}

void freeCampusData(Edge* edges, Node* nodes) {
  delete[] edges;
  delete[] nodes;
}
