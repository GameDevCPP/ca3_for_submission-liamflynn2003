//C++ file that handles movement of entities
#include "cmp_actor_movement.h"
#include <LevelSystem.h>
#include <engine.h>
using namespace sf;
using namespace std;

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
	: Component(p), _speed(80.0f), _moving(false), _direction(false) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}
//Moves the player
void ActorMovementComponent::move(const sf::Vector2f& p) {
	auto pp = _parent->getPosition() + p;
	if (validMove(pp)) {
		_parent->setPosition(pp);
	}
}

void ActorMovementComponent::adjustVerticalMovement(sf::Vector2f& pp, bool moveUp) {
	if (moveUp) {
		pp.y = _parent->getPosition().y + getSpeed(); // Move up
	} else {
		pp.y = _parent->getPosition().y - getSpeed(); // Move down
	}
}

void ActorMovementComponent::enemyMove(const sf::Vector2f& p) {
	auto pp = _parent->getPosition() + p;

	if (validMove(pp)) {
		_parent->setPosition(pp);
	} else {
		// Handle collision or invalid move by adjusting direction
		if (_parent->getPosition().x > pp.x) { // Enemy moving left
			adjustVerticalMovement(pp, _parent->getPosition().y > pp.y); // Move up or down based on player position
			pp.x = _parent->getPosition().x; // Prevent horizontal movement
		}

		else if (_parent->getPosition().x < pp.x) { // Enemy moving right
			adjustVerticalMovement(pp, _parent->getPosition().y > pp.y); // Move up or down based on player position
			pp.x = _parent->getPosition().x; // Prevent horizontal movement
		}

		else if (_parent->getPosition().y > pp.y) { // Enemy moving up
			pp.x = _parent->getPosition().x + getSpeed(); // Adjust to the right
			pp.y = _parent->getPosition().y; // Prevent vertical movement
		}

		else if (_parent->getPosition().y < pp.y) { // Enemy moving down
			pp.x = _parent->getPosition().x - getSpeed(); // Adjust to the left
			pp.y = _parent->getPosition().y; // Prevent vertical movement
		}

		// After adjusting for an obstacle, ensure the new position is valid
		if (validMove(pp)) {
			_parent->setPosition(pp);
		}
	}
}


//Takes an x and y value and puts it in a Vector2f to be used in the other move function
void ActorMovementComponent::move(float x, float y) {
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const { return _speed; }

void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }

bool ActorMovementComponent::getMoving() const { return _moving; }

void ActorMovementComponent::setMoving(bool moving) { _moving = moving; }

bool ActorMovementComponent::getDirection() { return _direction; }

void ActorMovementComponent::setDirection(bool direction) { _direction = direction; }