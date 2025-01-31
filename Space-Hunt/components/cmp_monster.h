// Header file for creating enemies
#pragma once

#include "cmp_actor_movement.h"
#include <ecm.h>

namespace sf
{
	class Texture;
}

class MonsterComponent : public ActorMovementComponent {
protected:
	int _health;
	std::shared_ptr<Entity> _player;
	std::shared_ptr<sf::Texture> _texture;
	float _attackTime;
	float _deathTime;
	int _damage;

public:
	MonsterComponent() = delete;
	explicit MonsterComponent(Entity* p, std::shared_ptr<Entity> player);

	void update(float dt) override;
	void render() override;
	~MonsterComponent() override = default;

	void set_health(int health);

	int get_health() const;

	void set_damage(int damage);

	void attack(float dt);
};
