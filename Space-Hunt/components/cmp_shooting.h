//Shooting component header file
#pragma once

#include "ecm.h"
#include "cmp_actor_movement.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Bullet final : sf::Sprite {

protected:
	void _update(double dt);
	bool isVisible{};
	sf::Vector2f mousePos;
	float angle{};
	int _damage;

	std::shared_ptr<sf::SoundBuffer> soundHit_buffer;
	std::shared_ptr<sf::Sound> soundHit;
	std::shared_ptr<sf::SoundBuffer> soundHitWall_buffer;
	std::shared_ptr<sf::Sound> soundHitWall;

public:
	static void update(double dt);
	static void render();
	static void fire(const sf::Vector2f& pos, Entity* parent);
	static void init();
	static void setAngle(float a, Bullet& b);

	~Bullet() override = default;
	Bullet();
	Entity* parentEntity{};
};

class ShootingComponent : public ActorMovementComponent {
protected:
	Entity* _parent;

	std::vector<Bullet> bullets;
	unsigned int bulletCount{};
	float angleshot{};

public:
	ShootingComponent() = delete;

	explicit ShootingComponent(Entity* p);

	void Fire() const;
	void update(float dt) override;
	void render() override;
};
