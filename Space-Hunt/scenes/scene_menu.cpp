#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../drop_pod_game.h"
#include "LevelSystem.h"
#include <iostream>
#include"../components/cmp_button.h"
#include "engine.h"
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

View menuView;

sf::Music music;

shared_ptr<Entity> btnExit;
shared_ptr<Entity> btnSetting;
shared_ptr<Entity> btnStart;

Sprite background;
bool shouldRenderBackground = true;
void MenuScene::Load() {
    cout << "Menu Load \n";

    menuView.reset(sf::FloatRect(0, 0, resolution.x, resolution.y));
    sf::RenderWindow& window = Engine::GetWindow();
    sf::Vector2u windowSize = window.getSize();

    // Set background
    try {
        auto backTexture = Resources::get<sf::Texture>("menu_bg.png");
        background.setTexture(*backTexture);

        // Get the background texture size
        sf::Vector2u textureSize = backTexture->getSize();

        // Calculate the scaling factors to fill the window
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        // Use the larger scale factor to ensure the image covers the entire window
        float scale = std::max(scaleX, scaleY);

        // Apply the scale to the background sprite
        background.setScale(scale, scale);

        // Optionally center the background if you want
        background.setPosition(
            (windowSize.x - textureSize.x * scale) / 2,
            (windowSize.y - textureSize.y * scale) / 2
        );
    } catch (const std::runtime_error& e) {
        std::cerr << "Resource error: " << e.what() << std::endl;
        shouldRenderBackground = false;
    }

    auto txt = makeEntity();
    auto pos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 5.0f);
    auto t = txt->addComponent<TextComponent>(menuView.getSize().x / 2.0f, menuView.getSize().y / 5.0f, "SPACE HUNTER");

    btnExit = makeEntity();
    auto btnPos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 1.5f);
    auto button = btnExit->addComponent<Button>(btnPos, "Exit", sf::Color::White, sf::Color::Green, sf::Color::Red);

    btnSetting = makeEntity();
    auto btn2Pos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 2.f);
    auto button2 = btnSetting->addComponent<Button>(btn2Pos, "Setting", sf::Color::White, sf::Color::Green, sf::Color::Red);

    btnStart = makeEntity();
    auto btn3Pos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 3.f);
    auto button3 = btnStart->addComponent<Button>(btn3Pos, "Play", sf::Color::White, sf::Color::Green, sf::Color::Red);

    // Ensure the music is loaded and playing
    auto musicstatus = music.getStatus();
    if (musicstatus == sf::SoundSource::Stopped || musicstatus == sf::SoundSource::Paused)
    {
        try
        {
            // Attempt to load the music file
            if (!music.openFromFile("res/sound/Title.wav"))
            {
                throw std::runtime_error("Failed to open music file");
            }

            music.setVolume(volume);
            music.setLoop(true);
            music.play();
        }
        catch (const std::exception& e)
        {
            // Catch and log any errors that occur during the music loading process
            std::cerr << "Error loading music: " << e.what() << std::endl;
        }
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    if (btnStart->GetCompatibleComponent<Button>()[0]->isPressed())
    {
        Engine::ChangeScene(&planetLevel);
        ls::setTextureMap("res/img/grass.png");
        music.stop();
    }
    else if (btnSetting->GetCompatibleComponent<Button>()[0]->isPressed())
    {
        Engine::ChangeScene(&settings);
    }
    else if (btnExit->GetCompatibleComponent<Button>()[0]->isPressed())
    {
        Engine::GetWindow().close();
    }

    if (!sf::Mouse::isButtonPressed(Mouse::Button::Left))
        Button::_mouseState = BUTTON_IDLE;

    Scene::Update(dt);
}

void MenuScene::Render()
{
    if(shouldRenderBackground) {
        Renderer::queue(&background);
    }
    Engine::setView(menuView);
    Scene::Render();
}