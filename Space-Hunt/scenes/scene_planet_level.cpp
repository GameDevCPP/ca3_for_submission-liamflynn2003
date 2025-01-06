#include "scene_planet_level.h"
#include "../space_hunt_game.h"
#include "engine.h"
#include "sound.h"
#include "../components/cmp_actor_movement.h"
#include "../components/cmp_player.h"
#include "../components/cmp_monster.h"
#include "../components/cmp_shooting.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_coin.h"
#include <LevelSystem.h>
#include <system_resources.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class SoundManager;
using namespace std;
using namespace sf;

void PlanetLevelScene::init()
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
    turnOffMusic = false;
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

void PlanetLevelScene::Load() {
    init();
    *ecm = Scene::getEcm();

    if(LevelSystem::currentLevel == 2) {
        hudView.setCenter(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
        ls::loadLevelFile("res/levels/levelTwoMap.txt");
    }
    else if(LevelSystem::currentLevel == 3) {
        hudView.setCenter(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
        ls::loadLevelFile("res/levels/levelThreeMap.txt");
    }
    else if(LevelSystem::currentLevel == 4) {
        hudView.setCenter(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
        ls::loadLevelFile("res/levels/levelFourMap.txt");
    }
    else if(LevelSystem::currentLevel == 5) {
        hudView.setCenter(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
        ls::loadLevelFile("res/levels/levelFiveMap.txt");
    }
    else {
    ls::loadLevelFile("res/levels/smallFloorMap.txt");
    }

    xCount = ls::getWidth();
    yCount = ls::getHeight();

    // Setting the center position and the size of the view.
    gameView.reset(sf::FloatRect(xCount * 100 * 0.5, yCount * 100 * 0.5, Engine::GetWindow().getSize().x, Engine::GetWindow().getSize().y));
    Engine::setView(gameView);

    startingCenter = gameView.getCenter();

    // Sound -----------------------------------------------------------------------
    soundShoot_buffer = Resources::get<SoundBuffer>("Shoot.wav");
    soundShoot = make_shared<Sound>(*soundShoot_buffer);
    soundShoot->setVolume(volume);

    // Player Entity ---------------------------------------------------------------

    playerRect = { Vector2i(0, 0), Vector2i(150, 150) };

    playerSpriteIdle = Resources::get<Texture>("Idle.png");
    playerSpriteMoving = Resources::get<Texture>("Run.png");

    player = makeEntity();
    std::vector<sf::Vector2ul> startTiles = ls::findTiles(LevelSystem::START);
    if (!startTiles.empty()) {
        // Assuming there is only one START tile, we can use the first result
        sf::Vector2ul startPos = startTiles[0];

        // Convert tile coordinates to world coordinates (assuming tile size is 100x100)
        sf::Vector2f playerStartPos(startPos.x * 100.f, startPos.y * 100.f);

        // Set the player's spawn position
        player->setPosition(playerStartPos);
    } else {
        // Handle case where no START tile is found (optional)
        std::cerr << "START tile not found in the level!" << std::endl;
        player->setPosition(startingCenter);
    }

    auto psprite = player->addComponent<SpriteComponent>();
    psprite->setTexture(playerSpriteIdle);

    psprite->getSprite().setOrigin(75, 75);
    psprite->getSprite().setScale(2, 2);

    auto panimation = player->addComponent<AnimationComponent>();
    panimation->setAnimation(8, 0.1, playerSpriteIdle, playerRect);

    auto pmove = player->addComponent<ActorMovementComponent>();
    pmove->setSpeed(200.f); // -----------------------------------------------------------------Player speed

    auto pattributes = player->addComponent<PlayerComponent>();

    auto pshooting = player->addComponent<ShootingComponent>();

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
    monsterCount = 1;
    damage = 2;  // Base damage

    if (LevelSystem::currentLevel == 2) {
        damage = 3;
    } else if (LevelSystem::currentLevel == 3) {
        damage = 4 ;
    } else if (LevelSystem::currentLevel >= 4) {
        damage = 5;
    }

    speed = 80;
    for (int i = 0; i < monsterCount; ++i)
    {
        SpawnEnemy(damage, speed);
    }

    // Coin load ----------------------------------------------------------------------
    int numberOfCoins = 0;
    if (LevelSystem::currentLevel == 2) {
        numberOfCoins = 10;
    } else if (LevelSystem::currentLevel == 3) {
        numberOfCoins = 15 ;
    } else if (LevelSystem::currentLevel >= 4) {
        numberOfCoins = 20;
    }
    SpawnCoins(numberOfCoins);

    // MUSIC -------------------------------------------------------------------------
    SoundManager& soundManager = SoundManager::getInstance();

    try {
        // Load and play the level music using SoundManager
        soundManager.loadMusic("Level", "res/sound/Level.wav");

        // Check if the music is already playing
        if (soundManager.getMusicStatus("Level") == sf::SoundSource::Stopped ||
            soundManager.getMusicStatus("Level") == sf::SoundSource::Paused)
        {
            soundManager.setMusicVolume("Level", 30);
            soundManager.setMusicLoop("Level", true);
            soundManager.playMusic("Level");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading or playing music: " << e.what() << std::endl;
    }

    // HUD ----------------------------------------------------------------------------
    timer->setString("Timer: 00:00");
    timer->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    timer->setCharacterSize(20);
    timer->setOutlineThickness(2);
    timer->setOrigin(timer->getLocalBounds().left + timer->getLocalBounds().width / 2.0f,
                     timer->getLocalBounds().top + timer->getLocalBounds().height / 2.0f);
    timer->setPosition(hudView.getSize().x * 0.5, 20);

    scoreText->setFont(*Resources::get<Font>("RobotoMono-Regular.ttf"));
    scoreText->setCharacterSize(20);
    scoreText->setOutlineThickness(2);
    scoreText->setPosition(hudView.getSize().x - 200, 20);

    // Level start message
    levelStartText->setString("LEVEL " + to_string(LevelSystem::currentLevel) + ": START!");
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

void PlanetLevelScene::UnLoad()
{
    Scene::UnLoad();
}

void PlanetLevelScene::Update(const double& dt) {
    if(turnOffMusic) {
        SoundManager& soundManager = SoundManager::getInstance();
        soundManager.stopMusic("Level");
    }
    if(seconds >= 2) {
        levelStart = false;
    }
    int scoreTarget = 100;
    if(LevelSystem::currentLevel == 2) {
        scoreTarget = 200;
    }
    else if(LevelSystem::currentLevel == 3) {
        scoreTarget = 200;
    }
    else if(LevelSystem::currentLevel == 4) {
        scoreTarget = 300;
    }
    else if(LevelSystem::currentLevel == 5) {
        scoreTarget = 300;
    }
    if (player->GetCompatibleComponent<PlayerComponent>()[0]->getScore() >= scoreTarget)
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
            speed++;
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

        auto playerScore = player->GetCompatibleComponent<PlayerComponent>()[0]->getScore();

        if(LevelSystem::currentLevel == 2) {
            scoreText->setString("SCORE: " + to_string(playerScore) + "/200");
        }
        else if(LevelSystem::currentLevel == 3) {
            scoreText->setString("SCORE: " + to_string(playerScore) + "/200");
        }
        else if(LevelSystem::currentLevel == 4) {
            scoreText->setString("SCORE: " + to_string(playerScore) + "/300");
        }
        else if(LevelSystem::currentLevel == 5) {
            scoreText->setString("SCORE: " + to_string(playerScore) + "/300");
        }
        else {
            scoreText->setString("SCORE: " + to_string(playerScore) + "/100");
        }


        auto playerHealth = player->GetCompatibleComponent<PlayerComponent>()[0]->getHealth();
        greenBar->setSize(Vector2f(playerHealth * 3, 30));
        healthText->setString("Health: " + to_string(playerHealth));

        Scene::Update(dt);
    }
    else
    {
        if (result == "win" && Keyboard::isKeyPressed(Keyboard::Enter)) {
            LevelSystem::currentLevel++;
            Engine::ChangeScene(&planetLevel);
        }

        else {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                Engine::ChangeScene(&menu);
                this->UnLoad();
            }
        }
        render_end();
    }

}

void PlanetLevelScene::Render() {
    // Set the view for the game scene
    Engine::setView(gameView);

    // Draw the game objects
    ls::renderFloor(Engine::GetWindow());
    Scene::Render();

    // Set the view for the HUD
    Engine::setView(hudView);
    std::cout << "HUD view set to: " << hudView.getCenter().x << ", " << hudView.getCenter().y << std::endl;

    // Draw the HUD elements
    Engine::GetWindow().draw(*timer);
    Engine::GetWindow().draw(*endText);
    Engine::GetWindow().draw(*endExitText);
    Engine::GetWindow().draw(*redBar);
    Engine::GetWindow().draw(*greenBar);
    Engine::GetWindow().draw(*healthText);
    Engine::GetWindow().draw(*scoreText);

    // Draw the level start text
    if(levelStart) {
        Engine::GetWindow().draw(*levelStartText);
    }
    Engine::setView(gameView);
}


void PlanetLevelScene::render_end() {
    if (result == "win")
    {
        if(LevelSystem::currentLevel == 5) {
            turnOffMusic = true;
            Engine::ChangeScene(&win);
        }

        int nextLevel = LevelSystem::currentLevel + 1;
        endText->setString("Level Complete!");
        endText->setOutlineColor(Color::Black);
        endText->setOutlineThickness(4);
        endText->setPosition(hudView.getSize().x / 2, 200);
        endText->setOrigin(endText->getLocalBounds().left + endText->getLocalBounds().width / 2.0f,
                           endText->getLocalBounds().top + endText->getLocalBounds().height / 2.0f);
        endExitText->setString("Press the ENTER button to go to Level " + to_string(nextLevel)+"!");
        endExitText->setOutlineColor(Color::Black);
        endExitText->setOutlineThickness(4);
        endExitText->setPosition(hudView.getSize().x * 0.5, 300);
        endExitText->setOrigin(endExitText->getLocalBounds().left + endExitText->getLocalBounds().width / 2.0f,
                               endExitText->getLocalBounds().top + endExitText->getLocalBounds().height / 2.0f);
    }
    if (result == "lose")
    {
        endText->setString("Defeat!");
        endText->setOutlineColor(Color::Black);
        endText->setOutlineThickness(4);
        endText->setPosition(hudView.getSize().x / 2, 200);
        endText->setOrigin(endText->getLocalBounds().left + endText->getLocalBounds().width / 2.0f,
                           endText->getLocalBounds().top + endText->getLocalBounds().height / 2.0f);
        endExitText->setString("Press the ENTER button to go back to menu!");
        endExitText->setOutlineColor(Color::Black);
        endExitText->setOutlineThickness(4);
        endExitText->setPosition(hudView.getSize().x * 0.5, 300);
        endExitText->setOrigin(endExitText->getLocalBounds().left + endExitText->getLocalBounds().width / 2.0f,
                               endExitText->getLocalBounds().top + endExitText->getLocalBounds().height / 2.0f);
    }


}

Vector2f PlanetLevelScene::random_position() const
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
void PlanetLevelScene::SpawnEnemy(int damage, float speed) {
    // Retrieve spawn tiles
    std::vector<sf::Vector2ul> spawnTiles = ls::findTiles(LevelSystem::SPAWN);
    if (spawnTiles.empty()) {
        std::cerr << "No spawn tiles found!" << std::endl;
        return;  // No spawn tiles available
    }

    IntRect enemyRect = { Vector2i(0, 0), Vector2i(64, 64) };
    shared_ptr<Texture> enemySprite;

    // Determine which texture to use based on the level
    if (LevelSystem::currentLevel > 3) {
        enemySprite = Resources::get<Texture>("Trash-Monster-Sprite-V2.png");
    } else {
        enemySprite = Resources::get<Texture>("Trash-Monster-Sheet.png");
    }

    // Loop through spawn tiles and spawn enemies
    for (size_t i = 0; i < spawnTiles.size(); ++i) {
        // Get the tile from the spawn list
        sf::Vector2ul spawnTile = spawnTiles[i % spawnTiles.size()];
        sf::Vector2f spawnPos(spawnTile.x * 100.f, spawnTile.y * 100.f);

        // Create the enemy entity
        shared_ptr<Entity> enemy = makeEntity();
        enemy->setPosition(spawnPos);

        // Set up enemy sprite and animation
        auto esprite = enemy->addComponent<SpriteComponent>();
        esprite->setTexture(enemySprite);
        esprite->getSprite().setScale(2, 2);

        auto eanimation = enemy->addComponent<AnimationComponent>();
        eanimation->setAnimation(6, 0.1, enemySprite, enemyRect);

        // Add movement component
        auto emove = enemy->addComponent<ActorMovementComponent>();
        emove->setMoving(true);
        emove->setSpeed(speed);

        // Add monster attributes
        auto eattributes = enemy->addComponent<MonsterComponent>(player);
        eattributes->set_damage(damage);

        // This is needed to have the enemy end at the player sprite.
        esprite->getSprite().setOrigin(32, 32);

        // Add the enemy tag
        enemy->addTag("enemy");
    }
}

void PlanetLevelScene::SpawnCoins(int numberOfCoins) {
    //  Minimum distance between coins
    float minDistance = 100.0f;

    // Get the tilemap's width and height in terms of tiles
    int mapWidth = ls::getWidth();
    int mapHeight = ls::getHeight();

    // Find all the positions of wall tiles
    auto wallTiles = ls::findTiles(LevelSystem::WALL);

    for (int i = 0; i < numberOfCoins; ++i) {
        bool validPosition = false;
        float x = 0, y = 0;

        while (!validPosition) {
            // Generate random positions in terms of tile indices
            int tileX = rand() % mapWidth;
            int tileY = rand() % mapHeight;
            // Adjust for tile size
            x = tileX * ls::getTileSize();
            y = tileY * ls::getTileSize();

            // Check if the new position is far enough from previously spawned coins
            validPosition = true;
            for (const auto& otherCoinPos : spawnedCoins) {
                float distX = x - otherCoinPos.x;
                float distY = y - otherCoinPos.y;
                float distance = sqrt(distX * distX + distY * distY);

                if (distance < minDistance) {
                    validPosition = false;
                    break;
                }
            }

            // Now, check if the position is a wall tile
            if (validPosition) {
                for (const auto& wallTile : wallTiles) {
                    // If the randomly generated position is on a wall tile, regenerate the position
                    if (wallTile.x == tileX && wallTile.y == tileY) {
                        validPosition = false;
                        break;  // Don't spawn on the wall tile
                    }
                }
            }
        }

        spawnedCoins.push_back(sf::Vector2f(x, y));

        // Create a new entity for the coin
        shared_ptr<Texture> coinSprite = Resources::get<Texture>("coin.png");
        shared_ptr<Entity> coin = makeEntity();
        auto csprite = coin->addComponent<SpriteComponent>();
        csprite->setTexture(coinSprite);
        csprite->getSprite().setScale(0.05, 0.05);

        // Set the coin's position
        coin->setPosition(sf::Vector2f(x, y));

        // Add CoinComponent to the entity and pass the player reference
        auto coinAttributes = coin->addComponent<CoinComponent>(player, 1);
    }
}

