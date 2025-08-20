/******************************************************************/
/*!
\file      AStarPathfinding.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Jan 14, 2025
\brief     This file contains the declarations for the AStarPathingFinding class along with the Node struct
           This file also contains the declarations for the member functions for both the class and the struct

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../ECS/Component/GridComponent.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

#ifndef ASTAR_H
#define ASTAR_H

struct Node {
    int x, y;                                       // Tile position
    float gCost = 0;                                // Cost from start to this node
    float hCost = 0;                                // Heuristic cost to the target
    float fCost() const { return gCost + hCost; }   // Total cost
    Node* parent = nullptr;                         // Pointer to the parent node for path reconstruction

    Node(int x, int y) : x(x), y(y) {}

    bool operator<(const Node& other) const {
        return fCost() > other.fCost(); // Reverse comparison for min-heap
    }
};

class AStarPathfinding {
public:
    std::vector<Node> FindPath(ecs::GridComponent* grid, int startX, int startY, int targetX, int targetY);

private:
    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0} // Up, Left, Down, Right
    };

    bool IsWalkable(ecs::GridComponent* grid, int x, int y);

    float CalculateHeuristic(int x1, int y1, int x2, int y2);

    std::vector<Node> ReconstructPath(Node* endNode);

    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
        }
    };
};

#endif // ASTAR_H