#include "cmp_coin.h"
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

CoinComponent::CoinComponent(Entity* p, std::shared_ptr<Entity> player, int value)
    : ActorMovementComponent(p),
      _player(player),
      _texture(nullptr),
      value(value)
{
}

void CoinComponent::update(double dt)
{
    // Get player position and coin position
    auto& playerPos = _player->getPosition();
    auto& coinPos = _parent->getPosition();

    // Calculate the distance between the player and the coin
    float xDistance = playerPos.x - coinPos.x;
    float yDistance = playerPos.y - coinPos.y;

    auto distance = (xDistance * xDistance) + (yDistance * yDistance);

    // Check if the player is within 100 units of the coin
    if (distance < 300 && _parent->isVisible())
    {
        auto playerComponent = _player->GetCompatibleComponent<PlayerComponent>();
        playerComponent[0]->addScore(20);
        soundCoin_buffer = Resources::get<sf::SoundBuffer>("Coin.wav");
        soundCoin = std::make_shared<sf::Sound>(*soundCoin_buffer);
        soundCoin->setVolume(volume);
        soundCoin->play();
        _parent->setVisible(false);
    }
}


