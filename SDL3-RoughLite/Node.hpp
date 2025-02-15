#pragma once

struct Node
{
    int x, y;
    float gCost, hCost, fCost;
    Node* parent;

    Node(int x, int y) : x(x), y(y), gCost(0), hCost(0), fCost(0), parent(nullptr) {}

    bool operator==(const Node& other) const
    {
        return x == other.x && y == other.y;
    }
};