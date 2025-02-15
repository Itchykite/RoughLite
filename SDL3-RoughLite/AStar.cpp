#include "AStar.hpp"
#include <iostream>

std::vector<Node> AStar::FindPath(Map* map, Node start, Node end)
{
    std::vector<Node> openList;
    std::vector<Node> closedList;
    openList.push_back(start);

    while (!openList.empty())
    {
        auto currentNode = std::min_element(openList.begin(), openList.end(), [](Node& a, Node& b) {
            return a.fCost < b.fCost;
            });

        Node current = *currentNode;
        openList.erase(currentNode);
        closedList.push_back(current);

        if (current == end)
        {
            std::vector<Node> path = RetracePath(&closedList.front(), &closedList.back());
            std::cout << "Path found: ";
            for (const auto& node : path)
            {
                std::cout << "(" << node.x << ", " << node.y << ") ";
            }
            std::cout << std::endl;
            return path;
        }

        for (Node& neighbor : GetNeighbors(map, current))
        {
            if (std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end())
            {
                continue;
            }

            float newGCost = current.gCost + std::sqrt(std::pow(current.x - neighbor.x, 2) + std::pow(current.y - neighbor.y, 2));
            if (newGCost < neighbor.gCost || std::find(openList.begin(), openList.end(), neighbor) == openList.end())
            {
                neighbor.gCost = newGCost;
                neighbor.hCost = CalculateHCost(neighbor, end);
                neighbor.fCost = neighbor.gCost + neighbor.hCost;
                neighbor.parent = &closedList.back();

                if (std::find(openList.begin(), openList.end(), neighbor) == openList.end())
                {
                    openList.push_back(neighbor);
                }
            }
        }
    }

    std::cout << "No path found" << std::endl;
    return std::vector<Node>();
}

float AStar::CalculateHCost(Node& node, Node& end)
{
    return std::abs(node.x - end.x) + std::abs(node.y - end.y);
}

std::vector<Node> AStar::GetNeighbors(Map* map, Node& node)
{
    std::vector<Node> neighbors;
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    for (auto& direction : directions)
    {
        int newX = node.x + direction[0];
        int newY = node.y + direction[1];

        if (map->IsNodeWalkable(newX, newY))
        {
            neighbors.emplace_back(newX, newY);
        }
    }

    return neighbors;
}

std::vector<Node> AStar::RetracePath(Node* startNode, Node* endNode)
{
    std::vector<Node> path;
    Node* currentNode = endNode;

    while (currentNode != startNode)
    {
        path.push_back(*currentNode);
        currentNode = currentNode->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}