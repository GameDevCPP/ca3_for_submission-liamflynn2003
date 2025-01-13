#ifndef ASTARNODE_H
#define ASTARNODE_H
#include <maths.h>

class AStarNode {
public:
    sf::Vector2ul position;
    float gCost = 0.0f;
    float hCost = 0.0f;
    float fCost = 0.0f;
    AStarNode* parent = nullptr;

    AStarNode(sf::Vector2ul pos, AStarNode* parentNode = nullptr, float g = 0.0f, float h = 0.0f)
        : position(pos), parent(parentNode), gCost(g), hCost(h) {}

    AStarNode(const AStarNode&) = delete;

    AStarNode(AStarNode&&) = delete;

    AStarNode& operator=(const AStarNode&) = delete;
    AStarNode& operator=(AStarNode&&) = delete;
};


struct AStarNodeComparator {
    bool operator()(const AStarNode* lhs, const AStarNode* rhs) const {
        return lhs->fCost > rhs->fCost;
    }
};

#endif