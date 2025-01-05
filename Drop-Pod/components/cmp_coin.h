// Header file: CoinComponent.h
#pragma once

#include "cmp_actor_movement.h"
#include <ecm.h>
#include <memory>
#include <SFML/Graphics.hpp>

namespace sf {
    class Texture;
}

class CoinComponent : public ActorMovementComponent {
public:
    CoinComponent(Entity* p, std::shared_ptr<Entity> player, int value = 1);
    int getValue() const;
    void setValue(int value);
    std::shared_ptr<Entity> _player;
    std::shared_ptr<sf::Texture> _texture;

private:
    int value;
};
