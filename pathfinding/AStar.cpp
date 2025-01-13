

#ifndef ASTAR_H
#define ASTAR_H

#include "AStar.h"
#include "AStarNode.h"
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <memory>
#include "../lib_tile_level_loader/LevelSystem.h"
#include <maths.h>

class CompareNode {
public:
    bool operator()(const AStarNode* lhs, const AStarNode* rhs) const {
        return lhs->fCost > rhs->fCost;
    }
};

AStar::AStar(){}

bool AStar::isWalkable(const sf::Vector2ul& pos) {
    // Use LevelSystem to check if the tile is walkable
    return _levelSystem->getTile(pos) != LevelSystem::WALL;
}

std::vector<AStarNode*> AStar::getNeighbors(AStarNode* node) {
    std::vector<AStarNode*> neighbors;
    sf::Vector2ul directions[] = {
        {1, 0}, {0, 1}
    };

    for (const auto& dir : directions) {
        sf::Vector2ul neighborPos = node->position + dir;
        if (isWalkable(neighborPos)) {
            neighbors.push_back(new AStarNode(neighborPos, node));
        }
    }
    return neighbors;
}

std::vector<sf::Vector2ul> AStar::findPath(const sf::Vector2ul& start, const sf::Vector2ul& end) {
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> openList;
    std::unordered_set<AStarNode*> closedList;
    std::unordered_set<AStarNode*> openSet;

    AStarNode* startNode = new AStarNode(start);
    openList.push(startNode);
    openSet.insert(startNode);

    while (!openList.empty()) {
        // Get the node with the lowest fCost
        AStarNode* current = openList.top();
        openList.pop();
        openSet.erase(current);

        if (current->position == end) {
            std::vector<sf::Vector2ul> path;
            while (current != nullptr) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());

            // Cleanup nodes
            for (auto node : openSet) {
                delete node;
            }
            for (auto node : closedList) {
                delete node;
            }

            return path;
        }

        closedList.insert(current);

        // Check the neighbors
        for (AStarNode* neighbor : getNeighbors(current)) {
            if (closedList.find(neighbor) != closedList.end()) {
                continue;  // Ignore the neighbor
            }

            float tentativeG = current->gCost + 1;
            if (tentativeG < neighbor->gCost || openSet.find(neighbor) == openSet.end()) {
                neighbor->gCost = tentativeG;
                neighbor->hCost = abs(static_cast<int>(neighbor->position.x) - static_cast<int>(end.x)) +
                                  abs(static_cast<int>(neighbor->position.y) - static_cast<int>(end.y));

                float newFCost = neighbor->gCost + neighbor->hCost;
                neighbor->fCost = newFCost;

                neighbor->parent = current;

                openList.push(neighbor);
                openSet.insert(neighbor);
            }
        }
    }

    // Cleanup nodes in case no path was found
    for (auto node : openSet) {
        delete node;
    }
    for (auto node : closedList) {
        delete node;
    }

    // No path found
    return {};
}

#endif