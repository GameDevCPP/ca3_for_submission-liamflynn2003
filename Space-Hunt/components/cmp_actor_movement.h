// Header file that handles movement of entities
#pragma once
#include <ecm.h>
#include "../pathfinding/AStar.h"

class ActorMovementComponent : public Component {
protected:
	static bool validMove(const sf::Vector2f&);
	float _speed;
	bool _moving;
	bool _direction;
	AStar _pathfinding;
public:
	void update(float dt) override;
	void move(const sf::Vector2f&) const;

	void adjustVerticalMovement(sf::Vector2f &pp, bool moveUp) const;

	void adjustHorizontalMovement(sf::Vector2f &pp, bool moveRight) const;

	void enemyMove(const sf::Vector2f&) const;
	void move(float x, float y) const;
	void render() override {}
	explicit ActorMovementComponent(Entity* p);
	ActorMovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float speed);

	bool getMoving() const;
	void setMoving(bool moving);

	bool getDirection() const;
	void setDirection(bool direction);
};