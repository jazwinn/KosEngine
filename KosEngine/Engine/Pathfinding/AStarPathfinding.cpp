/******************************************************************/
/*!
\file      AStarPathfinding.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Jan 14, 2025
\brief     This file contains the definition for the AStarPathingFinding class along with the Node struct
           This file also contains the definition for the member functions for both the class and the struct

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "AStarPathfinding.h"

//std::vector<Node> AStarPathfinding::FindPath(ecs::GridComponent* grid, int startX, int startY, int targetX, int targetY) {
//    std::priority_queue<Node> openSet;
//    std::unordered_map<std::pair<int, int>, Node*, PairHash> allNodes;
//
//    Node* startNode = new Node(startX, startY);
//    startNode->gCost = 0;
//    startNode->hCost = CalculateHeuristic(startX, startY, targetX, targetY);
//    openSet.push(*startNode);
//    allNodes[{startX, startY}] = startNode;
//
//    while (!openSet.empty()) {
//        Node currentNode = openSet.top();
//        openSet.pop();
//
//        if (currentNode.x == targetX && currentNode.y == targetY) {
//            return ReconstructPath(allNodes[{targetX, targetY}]);
//        }
//
//        for (const auto& dir : directions) {
//            int newX = currentNode.x + dir.first;
//            int newY = currentNode.y + dir.second;
//
//            if (!IsWalkable(grid, newX, newY)) continue;
//
//            float newGCost = currentNode.gCost + 1;
//
//            auto neighborKey = std::make_pair(newX, newY);
//            if (!allNodes.count(neighborKey) || newGCost < allNodes[neighborKey]->gCost) {
//                Node* neighborNode = new Node(newX, newY);
//                neighborNode->gCost = newGCost;
//                neighborNode->hCost = CalculateHeuristic(newX, newY, targetX, targetY);
//                neighborNode->parent = allNodes[{currentNode.x, currentNode.y}];
//                openSet.push(*neighborNode);
//                allNodes[neighborKey] = neighborNode;
//            }
//        }
//    }
//    return {}; // Return an empty path if no path is found
//}

#include <memory>

#include <memory>

std::vector<Node> AStarPathfinding::FindPath(ecs::GridComponent* grid, int startX, int startY, int targetX, int targetY) {
    auto cmp = [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
        return a->gCost + a->hCost > b->gCost + b->hCost;
        };
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> openSet(cmp);
    std::unordered_map<std::pair<int, int>, std::shared_ptr<Node>, PairHash> allNodes;

    auto startNode = std::make_shared<Node>(startX, startY);
    startNode->gCost = 0;
    startNode->hCost = CalculateHeuristic(startX, startY, targetX, targetY);
    openSet.push(startNode);
    allNodes[{startX, startY}] = startNode;

    while (!openSet.empty()) {
        std::shared_ptr<Node> currentNode = openSet.top();
        openSet.pop();

        if (currentNode.get()->x == targetX && currentNode.get()->y == targetY) {
            return ReconstructPath(allNodes[{targetX, targetY}].get());
        }

        for (const auto& dir : directions) {
            int newX = currentNode.get()->x + dir.first;
            int newY = currentNode.get()->y + dir.second;

            if (!IsWalkable(grid, newX, newY)) continue;

            float newGCost = currentNode.get()->gCost + 1;
            auto neighborKey = std::make_pair(newX, newY);

            if (!allNodes.count(neighborKey) || newGCost < allNodes[neighborKey]->gCost) {
                auto neighborNode = std::make_shared<Node>(newX, newY);
                neighborNode->gCost = newGCost;
                neighborNode->hCost = CalculateHeuristic(newX, newY, targetX, targetY);
                neighborNode->parent = allNodes[{currentNode.get()->x, currentNode.get()->y}].get();
                openSet.push(neighborNode);
                allNodes[neighborKey] = neighborNode;
            }
        }
    }
    return {}; // Return an empty path if no path is found
}


bool AStarPathfinding::IsWalkable(ecs::GridComponent* grid, int x, int y) {
    return x >= 0 && y >= 0 && x < grid->m_GridRowLength && y < grid->m_GridColumnLength && grid->m_IsWall[y][x] == 0;
}

float AStarPathfinding::CalculateHeuristic(int x1, int y1, int x2, int y2) {
    return static_cast<float>(std::abs(x1 - x2) + std::abs(y1 - y2)); // Manhattan distance
}

std::vector<Node> AStarPathfinding::ReconstructPath(Node* endNode) {
    std::vector<Node> path;
    Node* current = endNode;
    while (current) {
        path.push_back(*current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}