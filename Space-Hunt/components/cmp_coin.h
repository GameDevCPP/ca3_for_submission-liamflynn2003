#pragma once

#include "cmp_actor_movement.h"
#include <ecm.h>
#include <memory>
#include <SFML/Graphics.hpp>

#include "SFML/Audio/Sound.hpp"

namespace sf {
    class Texture;
}

class CoinComponent : public ActorMovementComponent {
public:
    CoinComponent(Entity* p, std::shared_ptr<Entity> player, int value = 1);

    void update(double dt);

    int getValue() const;
    void setValue(int value);
    std::shared_ptr<Entity> _player;
    std::shared_ptr<sf::Texture> _texture;
    std::shared_ptr<sf::SoundBuffer> soundCoin_buffer;
    std::shared_ptr<sf::Sound> soundCoin;
private:
    int value;
};
