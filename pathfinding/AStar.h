#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "AStarNode.h"
#include <maths.h>

class LevelSystem;

class AStar {
public:
    AStar();
    std::vector<sf::Vector2ul> findPath(const sf::Vector2ul& start, const sf::Vector2ul& end);

private:
    LevelSystem* _levelSystem;
    bool isWalkable(const sf::Vector2ul& pos);
    std::vector<AStarNode*> getNeighbors(AStarNode* node);
};
