//Player component header file
#pragma once

#include "cmp_actor_movement.h"
#include <ecm.h>

enum move_states { WSAD, Arrows };

class PlayerComponent : public ActorMovementComponent {
public:

	PlayerComponent() = delete;
	explicit PlayerComponent(Entity* p);

	void update(double dt) override;
	void render() override;
	~PlayerComponent() override = default;

	void setHealth(int health);
	int getHealth();

	void setScore(int score);
	void addScore(int score);
	int getScore();

protected:

	int _health;
	int _playerScore;
};