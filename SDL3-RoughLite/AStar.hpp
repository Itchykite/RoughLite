#pragma once
#include "Node.hpp"
#include "Map.hpp"
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

class AStar
{
public:
    static std::vector<Node> FindPath(Map* map, Node start, Node end);

private:
    static float CalculateHCost(Node& node, Node& end);
    static std::vector<Node> GetNeighbors(Map* map, Node& node);
    static std::vector<Node> RetracePath(Node* startNode, Node* endNode);
};