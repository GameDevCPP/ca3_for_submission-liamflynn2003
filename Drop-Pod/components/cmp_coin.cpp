#include "cmp_coin.h"

CoinComponent::CoinComponent(Entity* p, std::shared_ptr<Entity> player, int value)
    : ActorMovementComponent(p),
      _player(player),
      _texture(nullptr),
      value(value)
{
}

int CoinComponent::getValue() const {
    return value;
}

void CoinComponent::setValue(int value) {
    this->value = value;
}

