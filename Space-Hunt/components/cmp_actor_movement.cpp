//C++ file that handles movement of entities
#include "cmp_actor_movement.h"
#include <LevelSystem.h>
#include <engine.h>
using namespace sf;
using namespace std;

void ActorMovementComponent::update(const float dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
	: Component(p), _speed(80.0f), _moving(false), _direction(false) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}
//Moves the player
void ActorMovementComponent::move(const sf::Vector2f& p) const {
	auto pp = _parent->getPosition() + p;
	if (validMove(pp)) {
		_parent->setPosition(pp);
	}
}
void ActorMovementComponent::adjustVerticalMovement(sf::Vector2f& pp, const bool moveUp) const {
	if (moveUp) {
		pp.y = _parent->getPosition().y + getSpeed(); // Move up
	} else {
		pp.y = _parent->getPosition().y - getSpeed(); // Move down
	}
}

void ActorMovementComponent::adjustHorizontalMovement(sf::Vector2f& pp, bool moveRight) const {
	if (moveRight) {
		pp.x = _parent->getPosition().x + getSpeed(); // Move right
	} else {
		pp.x = _parent->getPosition().x - getSpeed(); // Move left
	}
}

void ActorMovementComponent::enemyMove(const sf::Vector2f& p) const {
	const auto currentPos = _parent->getPosition();  // Store current position
	auto pp = currentPos + p;

	if (validMove(pp)) {
		_parent->setPosition(pp);
		return;  // Early exit if move is valid
	}

	// Handle collision or invalid move by adjusting direction
	bool moveUp = currentPos.y > pp.y;
	bool moveRight = currentPos.x < pp.x;

	if (currentPos.x != pp.x) {  // Horizontal distance (left or right)
		adjustVerticalMovement(pp, moveUp);  // Move up or down based on player position
		pp.x = currentPos.x;  // Prevent horizontal movement
	}
	else if (currentPos.y != pp.y) { // Vertical distance (up or down)
		adjustHorizontalMovement(pp, currentPos.x > pp.x);  // Adjust horizontal position
		pp.y = currentPos.y;  // Prevent vertical movement
	}

	// After adjusting for an obstacle, ensure the new position is valid
	if (validMove(pp)) {
		_parent->setPosition(pp);
	}
}

//Takes an x and y value and puts it in a Vector2f to be used in the other move function
void ActorMovementComponent::move(float x, float y) const {
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const { return _speed; }

void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }

bool ActorMovementComponent::getMoving() const { return _moving; }

void ActorMovementComponent::setMoving(bool moving) { _moving = moving; }

bool ActorMovementComponent::getDirection() const { return _direction; }

void ActorMovementComponent::setDirection(bool direction) { _direction = direction; }