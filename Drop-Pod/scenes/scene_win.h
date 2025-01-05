#pragma once

#include "engine.h"
#include "../components/cmp_button.h"

class WinScene : public Scene {
public:
    WinScene() = default;
    ~WinScene() override = default;

    void Load() override;

    void Update(const double& dt) override;

    sf::View winView;

    std::shared_ptr<Entity> winBtnExit;

    sf::Sprite winBackground;

    sf::Music winMusic;
    sf::Texture texture;
    void Render() override;
};
