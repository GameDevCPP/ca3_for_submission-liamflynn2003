#include "cmp_coin.h"
#include "../space_hunt_game.h"
#include "cmp_actor_movement.h"
#include "cmp_player.h"
#include "engine.h"
#include "LevelSystem.h"
#include "sound.h"

CoinComponent::CoinComponent(Entity* p, std::shared_ptr<Entity> player, int value)
    : ActorMovementComponent(p),
      _player(player),
      _texture(nullptr),
      value(value)
{
    SoundManager& soundManager = SoundManager::getInstance();
    try {
        soundManager.loadSoundEffect("Coin", "res/sound/Coin.wav");
    } catch (const std::exception& e) {
        std::cerr << "Error loading sound effect: " << e.what() << std::endl;
    }
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

    // Check if the player is within 350 units of the coin
    if (distance < 350 && _parent->isVisible())
    {
        auto playerComponent = _player->GetCompatibleComponent<PlayerComponent>();
        playerComponent[0]->addScore(20);

        // SFX
        SoundManager& soundManager = SoundManager::getInstance();
        try {
            soundManager.playSoundEffect("Coin", volume);
        } catch (const std::exception& e) {
            std::cerr << "Error playing sound effect: " << e.what() << std::endl;
        }
        _parent->setVisible(false);
    }
}


