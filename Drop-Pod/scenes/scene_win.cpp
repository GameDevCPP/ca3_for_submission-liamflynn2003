#include "scene_win.h"
#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../drop_pod_game.h"
#include "LevelSystem.h"
#include <iostream>
#include "../components/cmp_button.h"
#include "engine.h"
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

View winView;

sf::Music winMusic;

shared_ptr<Entity> winBtnExit;

Sprite winBackground;
bool renderBackground = true;

void WinScene::Load() {
    cout << "Win Screen Load \n";

    winView.reset(sf::FloatRect(0, 0, resolution.x, resolution.y));
    sf::RenderWindow& window = Engine::GetWindow();
    sf::Vector2u windowSize = window.getSize();

    // Set background
    try {
        auto backTexture = Resources::get<sf::Texture>("win.png");
        winBackground.setTexture(*backTexture);

        // Get the background texture size
        sf::Vector2u textureSize = backTexture->getSize();

        // Calculate the scaling factors to fill the window
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        // Use the larger scale factor to ensure the image covers the entire window
        float scale = std::max(scaleX, scaleY);

        // Apply the scale to the background sprite
        winBackground.setScale(scale, scale);

        // Optionally center the background if you want
        winBackground.setPosition(
            (windowSize.x - textureSize.x * scale) / 2,
            (windowSize.y - textureSize.y * scale) / 2
        );
    } catch (const std::runtime_error& e) {
        std::cerr << "Resource error: " << e.what() << std::endl;
        renderBackground = false;
    }

    // "CONGRATULATIONS!" text
    auto congratsText = makeEntity();
    auto pos = Vector2f(winView.getSize().x / 2.0f, winView.getSize().y / 4.0f);
    auto text = congratsText->addComponent<TextComponent>(pos.x, pos.y, "CONGRATULATIONS!");

    // Additional text below "CONGRATULATIONS!"
    auto additionalText = makeEntity();
    auto additionalPos = Vector2f(winView.getSize().x / 2.0f, pos.y + 50); // Adjust Y offset as needed
    auto additionalTextComponent = additionalText->addComponent<TextComponent>(
        additionalPos.x, additionalPos.y, "You have completed your hunt and escaped alive."
    );

    // Exit button (back to the main menu)
    winBtnExit = makeEntity();
    auto btnPos = Vector2f(winView.getSize().x / 2.0f, winView.getSize().y / 1.5f);
    auto button = winBtnExit->addComponent<Button>(btnPos, "Back to Menu", sf::Color::White, sf::Color::Green, sf::Color::Red);

    // Load music (if not already playing)
    auto musicstatus = winMusic.getStatus();
    if (musicstatus == SoundSource::Stopped || musicstatus == SoundSource::Paused) {
        if (!winMusic.openFromFile("res/sound/win.wav")) {
            std::cerr << "Music broken" << std::endl;
        }
        winMusic.setVolume(volume);
        winMusic.setLoop(true);
        winMusic.play();
    }

    setLoaded(true);
}

void WinScene::Update(const double& dt) {
    // If the back button is pressed, go back to the main menu
    if (winBtnExit->GetCompatibleComponent<Button>()[0]->isPressed()) {
        Engine::ChangeScene(&menu);
        winMusic.stop();
    }

    // Reset button state when the mouse is not pressed
    if (!sf::Mouse::isButtonPressed(Mouse::Button::Left)) {
        Button::_mouseState = BUTTON_IDLE;
    }

    Scene::Update(dt);
}

void WinScene::Render() {
    if (renderBackground) {
        Renderer::queue(&winBackground);
    }
    Engine::setView(winView);
    Scene::Render();
}
