//
// Created by Liam on 04/01/2025.
//

#include "scene_planet_level2.h"
#include "scene_planet_level.h"
#include "../drop_pod_game.h"
#include "engine.h"
#include "../components/cmp_actor_movement.h"
#include "../components/cmp_player.h"
#include "../components/cmp_monster.h"
#include "../components/cmp_shooting.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include <LevelSystem.h>
#include <system_resources.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>


using namespace std;
using namespace sf;

void PlanetLevel2Scene::init()
{
    ecm = new EntityManager();

    gameView = Engine::GetWindow().getView();
    hudView = Engine::GetWindow().getView();

    // Level global variables
    speed = 0;
    xCount = 0;
    yCount = 0;
    startingCenter = Vector2f(0, 0);
    viewToggle = false;
    pauseGame = false;
    levelStart = true;
    result = "Not Set";

    // Player Variables
    player = makeEntity();
    playerSpriteIdle = make_shared<Texture>();
    playerSpriteMoving = make_shared<Texture>();
    playerRect = IntRect();

    soundShoot_buffer = make_shared<SoundBuffer>();
    soundShoot = make_shared<Sound>();

    redBar = new RectangleShape();
    greenBar = new RectangleShape();
    healthText = new Text();

    // Enemy Variables
    totalTime = 0.f;

    // Actual Hud
    tempTime = 0;
    minutes = 0;
    seconds = 0;

    timer = new Text();
    endText = new Text();
    endExitText = new Text();
    scoreText = new Text();
    levelStartText = new Text();

    // Shooting Delay
    fireTime = 0.f;
}

void PlanetLevel2Scene::Load() {
    init();
    *ecm = Scene::getEcm();

    ls::loadLevelFile("res/levels/smallfloorMap.txt");
    //ls::loadLevelFile("res/levels/floorMap.txt");
    xCount = ls::getWidth();
    yCount = ls::getHeight();

    // Setting the center position and the size of the view.
    gameView.reset(sf::FloatRect(xCount * 100 * 0.5, yCount * 100 * 0.5, Engine::GetWindow().getSize().x, Engine::GetWindow().getSize().y));
    Engine::setView(gameView);

    startingCenter = gameView.getCenter();

    // Sound -----------------------------------------------------------------------
    soundShoot_buffer = Resources::get<SoundBuffer>("Shoot_001.wav");
    soundShoot = make_shared<Sound>(*soundShoot_buffer);
    soundShoot->setVolume(volume);

    // Player Entity ---------------------------------------------------------------

    playerRect = { Vector2i(0, 0), Vector2i(150, 150) };

    playerSpriteIdle = Resources::get<Texture>("Idle.png");
    playerSpriteMoving = Resources::get<Texture>("Run.png");

    player = makeEntity();
    //player->setPosition(Vector2f(view.getSize().x * 0.5, view.getSize().y * 0.5));
    player->setPosition(startingCenter);

    auto psprite = player->addComponent<SpriteComponent>();
    psprite->setTexture(playerSpriteIdle);

    auto pspriteBounds = Vector2f(psprite->getSprite().getTextureRect().width * 0.5f, psprite->getSprite().getTextureRect().height * 0.5f);
    psprite->getSprite().setOrigin(75, 75);
    psprite->getSprite().setScale(2, 2);

    auto panimation = player->addComponent<AnimationComponent>();
    panimation->setAnimation(8, 0.1, playerSpriteIdle, playerRect);

    auto pmove = player->addComponent<ActorMovementComponent>();
    pmove->setSpeed(200.f); // -----------------------------------------------------------------Player speed

    auto pattributes = player->addComponent<PlayerComponent>();

    auto pshooting = player->addComponent<ShootingComponent>();

    auto score = player->GetCompatibleComponent<PlayerComponent>()[0]->getScore();

    // Health Bar ----------------------------------------------------------------------

    redBar->setSize(Vector2f(300, 30));
    redBar->setFillColor(Color::Red);
    redBar->setPosition(20, hudView.getSize().y - 50);

    greenBar->setSize(Vector2f(300, 30));
    greenBar->setFillColor(Color::Green);
    greenBar->setPosition(20, hudView.getSize().y - 50);

    healthText->setString("Health: 100");
    healthText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    healthText->setCharacterSize(18);
    healthText->setOutlineThickness(2);
    healthText->setPosition(160, hudView.getSize().y - 35);
    healthText->setOrigin(healthText->getLocalBounds().left + healthText->getLocalBounds().width / 2.0f,
                          healthText->getLocalBounds().top + healthText->getLocalBounds().height / 2.0f);

    // Enemies Load --------------------------------------------------------------------
    monsterCount = 5;
    damage = 2;
    speed = 80;
    for (int i = 0; i < monsterCount; ++i)
    {
        SpawnEnemy(damage, speed);
    }

    // HUD ----------------------------------------------------------------------------
    timer->setString("Timer: 00:00");
    timer->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    timer->setCharacterSize(20);
    timer->setOutlineThickness(2);
    timer->setOrigin(timer->getLocalBounds().left + timer->getLocalBounds().width / 2.0f,
                     timer->getLocalBounds().top + timer->getLocalBounds().height / 2.0f);
    timer->setPosition(hudView.getSize().x * 0.5, 20);

    scoreText->setString("SCORE: 0000");
    scoreText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    scoreText->setCharacterSize(20);
    scoreText->setOutlineThickness(2);
    scoreText->setPosition(hudView.getSize().x - 150, 20);

    // Level start message
    levelStartText->setString("LEVEL 1: START!");
    levelStartText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    levelStartText->setCharacterSize(80);
    levelStartText->setFillColor(Color::White);
    levelStartText->setOutlineThickness(5);
    levelStartText->setOutlineColor(Color::Black);
    levelStartText->setOrigin(levelStartText->getLocalBounds().left + levelStartText->getLocalBounds().width / 2.0f,
                             levelStartText->getLocalBounds().top + levelStartText->getLocalBounds().height / 2.0f);
    levelStartText->setPosition(hudView.getSize().x * 0.5, 80);

    endText->setString("");
    endText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    endText->setCharacterSize(50);

    endExitText->setString("");
    endExitText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    endExitText->setCharacterSize(30);

    // Set load to true when finished.
    setLoaded(true);
}

void PlanetLevel2Scene::UnLoad()
{
    Scene::UnLoad();
}

void PlanetLevel2Scene::Update(const double& dt) {
    if(seconds >= 2) {
        levelStart = false;
    }
    if (player->GetCompatibleComponent<PlayerComponent>()[0]->getScore() > 200)
    {
        pauseGame = true;
        result = "win";
    }

    if (player->isAlive() == false)
    {
        pauseGame = true;
        result = "lose";
    }

    if (!pauseGame)
    {
        // Player updates -----------------------------------------------------------------------------------------------
        fireTime -= dt;

        if (fireTime <= 0 && Mouse::isButtonPressed(Mouse::Left)) {
            player->GetCompatibleComponent<ShootingComponent>()[0]->Fire();
            fireTime = 0.5f;
            soundShoot->play();
        }

        if (Keyboard::isKeyPressed(Keyboard::P))
        {
            player->GetCompatibleComponent<PlayerComponent>()[0]->setHealth(0);
        }

        gameView.setCenter(player->getPosition());

        // Enemies spawning Timeline -------------------------------------------------------------------------------------

        totalTime += dt;
        if (totalTime >= 20)
        {
            monsterCount++;
            damage++;
            speed += 7;
            for (int i = 0; i < monsterCount; ++i)
            {
                SpawnEnemy(damage, speed);
            }
            totalTime = 0;
        }

        // HUD update -----------------------------------------------------------------------------
        tempTime += dt;
        if (tempTime >= 1) { seconds++; tempTime = 0.f; }
        if (seconds == 60) { minutes++; seconds = 0; }

        string sec;
        string min;
        if (seconds < 10) { sec = "0" + to_string(seconds); }
        else { sec = to_string(seconds); }

        if (minutes < 10) { min = "0" + to_string(minutes); }
        else { min = to_string(minutes); }

        string s = ("Timer: " + min + ":" + sec);
        timer->setString(s);

        string scoreString = "0001";

        auto playerScore = player->GetCompatibleComponent<PlayerComponent>()[0]->getScore();
        string score = ("SCORE: " + scoreString);
        scoreText->setString(to_string(playerScore));

        auto playerHealth = player->GetCompatibleComponent<PlayerComponent>()[0]->getHealth();
        greenBar->setSize(Vector2f(playerHealth * 3, 30));
        healthText->setString("Health: " + to_string(playerHealth));

        Scene::Update(dt);
    }
    else
    {
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            Engine::ChangeScene(&menu);
            this->UnLoad();
        }

        render_end();
    }
}

void PlanetLevel2Scene::Render() {
    // Set the view for the game scene
    Engine::setView(gameView);

    // Draw the game objects (floor, etc.)
    ls::renderFloor(Engine::GetWindow());
    Scene::Render();

    // Set the view for the HUD
    Engine::setView(hudView);

    // Draw the HUD elements
    Engine::GetWindow().draw(*timer);
    Engine::GetWindow().draw(*endText);
    Engine::GetWindow().draw(*endExitText);
    Engine::GetWindow().draw(*redBar);
    Engine::GetWindow().draw(*greenBar);
    Engine::GetWindow().draw(*healthText);
    Engine::GetWindow().draw(*scoreText);
    // Draw the level start text if necessary
    if(levelStart) {
        Engine::GetWindow().draw(*levelStartText);
    }
    Engine::setView(gameView);
}


void PlanetLevel2Scene::render_end() const
{
    if (result == "win")
    {
        endText->setString("Victory!");
        endText->setOutlineColor(Color::Black);
        endText->setOutlineThickness(4);
        endText->setPosition(hudView.getSize().x / 2, 200);
        endText->setOrigin(endText->getLocalBounds().left + endText->getLocalBounds().width / 2.0f,
                           endText->getLocalBounds().top + endText->getLocalBounds().height / 2.0f);
    }
    if (result == "lose")
    {
        endText->setString("Defeat!");
        endText->setOutlineColor(Color::Black);
        endText->setOutlineThickness(4);
        endText->setPosition(hudView.getSize().x / 2, 200);
        endText->setOrigin(endText->getLocalBounds().left + endText->getLocalBounds().width / 2.0f,
                           endText->getLocalBounds().top + endText->getLocalBounds().height / 2.0f);
    }

    endExitText->setString("Press the ENTER button to go back to menu!");
    endExitText->setOutlineColor(Color::Black);
    endExitText->setOutlineThickness(4);
    endExitText->setPosition(hudView.getSize().x * 0.5, 300);
    endExitText->setOrigin(endExitText->getLocalBounds().left + endExitText->getLocalBounds().width / 2.0f,
                           endExitText->getLocalBounds().top + endExitText->getLocalBounds().height / 2.0f);
}

Vector2f PlanetLevel2Scene::random_position() const
{
    auto viewSize = gameView.getSize();
    auto viewCenter = gameView.getCenter();

    int xSize = viewSize.x;
    auto randNumberX = rand() % xSize + (-viewSize.x * 0.5);

    int ySize = viewSize.y;
    auto randNumberY = rand() % ySize + (-viewSize.y * 0.5);

    int number = rand() % 3;
    switch (number)
    {
        case 0:
            return Vector2f(viewCenter.x + randNumberX, viewCenter.y - viewSize.y * 0.5 - 100);
        case 1:
            return Vector2f(viewCenter.x + randNumberX, viewCenter.y + viewSize.y * 0.5 + 100);
        case 2:
            return Vector2f(viewCenter.x - viewSize.x * 0.5 - 100, viewCenter.y + randNumberY);
        case 3:
            return Vector2f(viewCenter.x + viewSize.x * 0.5 + 100, viewCenter.y + randNumberY);
    }
    return {};
}

//Creates and enemy and adds it to the entity list for the scene.
void PlanetLevel2Scene::SpawnEnemy(int damage, float speed)
{
    IntRect enemyRect = { Vector2i(0, 0), Vector2i(64, 64) };
    shared_ptr<Texture> enemySprite = Resources::get<Texture>("Trash-Monster-Sheet.png");
    shared_ptr<Entity> enemy = makeEntity();

    // Set random position outside of view.
    auto pos = random_position();
    enemy->setPosition(pos);

    auto esprite = enemy->addComponent<SpriteComponent>();
    esprite->setTexture(enemySprite);
    esprite->getSprite().setScale(2, 2);

    auto eanimation = enemy->addComponent<AnimationComponent>();
    eanimation->setAnimation(6, 0.1, enemySprite, enemyRect);

    auto emove = enemy->addComponent<ActorMovementComponent>();
    emove->setMoving(true);
    emove->setSpeed(speed);

    auto eattributes = enemy->addComponent<MonsterComponent>(player);
    eattributes->set_damage(damage);

    // This is needed to have the enemy end at the player sprite.
    esprite->getSprite().setOrigin(32, 32);

    enemy->addTag("enemy");
}