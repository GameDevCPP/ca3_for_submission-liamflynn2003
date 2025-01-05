// Shooting component C++ file
#include "cmp_shooting.h"
#include "../drop_pod_game.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include <SFML/Graphics.hpp>

#include "cmp_monster.h"
#include "cmp_player.h"
#include "system_renderer.h"
#include "engine.h"
#include "LevelSystem.h"

using namespace std;
using namespace sf;

// ShootingComponent Variables
unsigned int bulletCount;
vector<Bullet> bullets(256);

float tempfloat = 0.f;
shared_ptr<float> angleshot = make_unique<float>(tempfloat);

// Bullet Variables
bool isVisible = false;
float angle;
Vector2f mousePos;

Texture temp2;
auto spriteTexture = make_shared<Texture>(temp2);

bool validMove(const sf::Vector2f& pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

ShootingComponent::ShootingComponent(Entity* p) : ActorMovementComponent(p), _parent(p), bulletCount(0), angleshot(0) {
	if (_parent == nullptr) {
		std::cerr << "ShootingComponent parent is nullptr!" << std::endl;
	}
	Bullet::init();
}


void ShootingComponent::update(double dt) {
	Bullet::update(dt);
}

void ShootingComponent::render() {
	Bullet::render();
}

void ShootingComponent::Fire() {
	auto spriteSize = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getLocalBounds();
	Vector2f spriteCenter = Vector2f(spriteSize.width * 0.5, spriteSize.height * 0.5);
	// Firing the bullets
	Bullet::fire(_parent->getPosition(), _parent);
}

Bullet::Bullet()
{
	_damage = 34;
}

void Bullet::init() {
	spriteTexture = Resources::get<Texture>("arrow.png");

	for (auto& b : bullets) {
		b.setPosition(Vector2f(-100, -100));
		b.setTexture(*spriteTexture);
		b.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(spriteTexture->getSize().x, spriteTexture->getSize().y)));
		b.setOrigin(Vector2f(spriteTexture->getSize().x * 0.5, spriteTexture->getSize().y * 0.5));
		b.setAngle(0.f, b);
		b.mousePos = Vector2f(0, 0);
		b.isVisible = false;
	}
}

void Bullet::setAngle(float a, Bullet& b) {
	b.angle = a;
}

void Bullet::update(const double dt) {
	for (auto& b : bullets) {
		b._update(dt);
	}
}

void Bullet::render() {
	for (auto& b : bullets) {
		if (b.isVisible) {
			Renderer::queue(&b);
		}
	}
}
void Bullet::fire(const sf::Vector2f& pos, Entity* parent) {
	sf::RenderWindow& window = Engine::GetWindow();
	auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	bulletCount++;
	bulletCount = bulletCount % 256;
	bullets[bulletCount].setPosition(pos);
	bullets[bulletCount].isVisible = true;
	bullets[bulletCount].parentEntity = parent;

	sf::Vector2f direction = mousePos - pos;
	direction = sf::normalize(direction);  // Ensure the direction vector is normalized

	// Calculate the angle in degrees
	float angleDeg = atan2(direction.y, direction.x) * 180.0f / 3.14159265358979323846f;

	// Adjust the rotation by subtracting 90 degrees
	bullets[bulletCount].setRotation(angleDeg - 10);
	bullets[bulletCount].setAngle(atan2(direction.y, direction.x), bullets[bulletCount]);
	bullets[bulletCount].setScale(0.02f, 0.02f);
}

void Bullet::_update(const double dt) {
    RenderWindow& window = Engine::GetWindow();
    const View view = window.getView();

    // If bullet is out of bounds, remove/return
    if (getPosition().x < view.getCenter().x - 100 - view.getSize().x * 0.5 || getPosition().x > view.getCenter().x + 100 + view.getSize().x * 0.5
        || getPosition().y < view.getCenter().y - 100 - view.getSize().y * 0.5 || getPosition().y > view.getCenter().y + 100 + view.getSize().y * 0.5)
    {
        this->isVisible = false;
        return;
    }

    // Calculate the potential new position
    Vector2f newPos = getPosition();
    newPos.x += cos(this->angle) * 200.f * dt;
    newPos.y += sin(this->angle) * 200.f * dt;

    // Check if the new position is valid (not hitting a wall)
    if (validMove(newPos)) {
        setPosition(newPos);
    } else {
        // If not a valid move (hit a wall), destroy the bullet
    	soundHitWall_buffer = Resources::get<SoundBuffer>("HitWall.wav");
    	soundHitWall = make_shared<Sound>(*soundHitWall_buffer);
    	soundHitWall->setVolume(volume);
    	soundHitWall->play();
        this->isVisible = false;
        setPosition(-100, -100);  // Move the bullet off-screen to "destroy" it
        return;  // Exit the function since the bullet is destroyed
    }

    // Check for collisions with enemies
    auto ecm = planetLevel.getEcm();
    auto enemies = ecm.find("enemy");
    auto player = ecm.find("player");
    auto boundingBox = getGlobalBounds();

    for (auto enemy : enemies) {
        auto sprite = enemy->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
        auto spriteBounds = sprite.getGlobalBounds();
        spriteBounds.top += 40;
        spriteBounds.left += 40;
        spriteBounds.width -= 70;
        spriteBounds.height -= 70;

        if (enemy->isAlive() && spriteBounds.intersects(boundingBox)) {
            this->isVisible = false;
            setPosition(-100, -100);  // Bullet disappears after hitting enemy

            // Hit Sound
            soundHit_buffer = Resources::get<SoundBuffer>("Hit.wav");
            soundHit = make_shared<Sound>(*soundHit_buffer);
            soundHit->setVolume(volume);
            soundHit->play();

            auto currentHealth = enemy->GetCompatibleComponent<MonsterComponent>()[0]->get_health();
            std::cout << "parent: " << parentEntity << std::endl;

            // Check if the enemy's health is 0 or less and update score
            if ((enemy->GetCompatibleComponent<MonsterComponent>()[0]->get_health() - _damage) <= 0) {
                auto playerComponent = parentEntity->GetCompatibleComponent<PlayerComponent>();
                if (!playerComponent.empty()) {
                    // Log various player attributes
                    std::cout << "Player Score: " << playerComponent[0]->getScore() << std::endl;
                    std::cout << "Player Health: " << playerComponent[0]->getHealth() << std::endl;
                    playerComponent[0]->addScore(10);  // Add 10 points for shooting
                }
            }
            enemy->GetCompatibleComponent<MonsterComponent>()[0]->set_health(currentHealth - _damage);
        }
    }
}

