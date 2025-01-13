# Game Design document

## Introduction
This document outlines "Space Hunt," an exciting 2D space shooter where players navigate through a series of five distinct levels, collecting points by gathering coins and defeating enemies to progress. The game combines intense gameplay with a showcase of essential C++ game development techniques.

"Space Hunt" uses tilemaps with texture mapping to create environments with floors, walls and enemy spawners. Players must maneuver through these levels, surviving monsters that hunt them down, and defeating them to progress to the next level. The game also features a custom-built sound library, used for managing sound effects and background music.

## Technology

### Platforms
"Space Hunt" is designed to run on multiple platforms, including:
- **Windows**
- **macOS**
- **Linux**

### Game Engine/Software Library

The game is built using a combination of external libraries and custom modules, each contributing to different aspects of the game's functionality:

- **SFML (Simple and Fast Multimedia Library)**: Handles graphics rendering, window management, input processing.

- **lib_ecm (Entity Component Management)**: Manages game entities and components, enabling flexible entity management.

- **lib_maths**: Provides essential mathematical functions and utilities for calculations related to game physics, transformations, and other numerical operations.

- **lib_sound**: A custom sound management system designed for efficient handling of sound effects and background music.

- **lib_tile_level_loader**: Manages the game's tile system, including loading and rendering tilemaps, applying textures, and handling environmental elements like floors, walls, and enemy spawners.

- **Engine**: Responsible for managing system resources, rendering scenes, and handling game state transitions. This engine integrates the various libraries and systems into a cohesive framework.

 - **Physics and Collision Detection**: The game uses Box2D for physics simulation and collision detection, handling interactions between game entities like the player, enemies, and environmental objects.

## Backstory
In a world where humanity explores the stars, you play as a hunter whose ship has crash landed on a strange planet. Dedicated to your duties, you resolve to make the most of your unwanted trip - by hunting the local fauna and collecting treasure as you survive the hostile planet and attempt escape.

Having ejected from his ship, the hunter is only left with rudimentary arrow weapons which he crafted. With these arrows as his only weapons, and with monster alien wolves hunting him at every step, the hunter begins his journey.

## Objective
The player must increase their score by hunting wolves and collecting coins on each level.
Once a certain score is reached, the player will proceed to the next level, which varies in visual design and layout.
Each level increases in difficulty, and once the player completes the fifth and final level, the hunt is complete, the hunter escapes the planet and the player views a win screen.

## Gameplay
Players spawn on a set spawn tile in each level. Level One features no coins, but 4 enemies will spawn and run towards the player.
The player character, the Hunter, has 100 hp, and each enemy attack reduces it by 2 hp - once an enemy reaches the player, they hunter, they will attack.
If the player reaches 0 HP, the game will pause, and "Game Over" text will be displayed. the player can then press the enter key to return to the title screen.

Each wolf kill provides the player with 10 score, and each coin they collect provides the player with 20 score, which is displayed in the HUD.
Wolves spawn from "enemy spawn" tiles, which are location in certain locations in each level. Once spawned, they will begin to move towards the player.
Wolves spawn every 20 seconds and move around walls if they block access to the player.
Levels become increasingly more mazelike as the game progresses, making coins harder to find, and making it easier to be caught by the wolves and pinned in a dead-end / corner.
Coins spawn at random locations in each level, from Level 2 onwards. They cannot spawn too close to each other, and can only spawn on floor tiles.

## Levels
### Level 1
On level 1, there are 4 enemy spawners, which spawn enemy wolves every 20 seconds. The player must reach 100 score on Level 1 to proceed to Level 2.
The player spawns in the middle of the level, and it does not feature many walls, allowing the player to learn the mechanics on an easy level.

### Level 2
Level 2 introduces coins, which can be collected to increase a player's score by 20. This level is vertically larger than level one but the same width, and features 6 enemy spawn points.
There are blocks of walls around the level, and the player spawns in the centre. As the player discovers coins, they will learn that exploring levels to search for coins will help to complete levels faster, before too many wolves spawn.

### Level 3
Level 3 is smaller vertically and larger horizontally then the previous levels.
It features 8 wolf spawn points, and spawns the player at one side of the map, which encourages the player to move deeper into the level to hunt for more wolves and coins.

### Level 4
Level 4 is a maze-like level, which is vertically large and spawns the player at the bottom. The player has to move up through the level to find coins and wolves, and it features corners and deadends within the maze, which the player could get trapped in if they navigate poorly and are chased by wolves. the level features 7 wolf spawn points.

### Level 5
Level 5 is a series of tight mazelike corridors, with many wolf spawn points. Players will have to dodge wolves in these tight spaces while collecting coins and defeating the wolves to increase their score.
Like the last level, the player will have to navigate through the level towards more enemy spawns to collet coins, so they can complete the level before getting overwhelmed. It serves as a final challenge before completing the game.

## Controls
Players use WASD to move, and left click to shoot. This player has the option to switch to using arrow keys for movement in the settings menu.

## GUI/In-game menu
### Splash Screen + Menu
![image](https://github.com/user-attachments/assets/75fd073e-8535-4d1c-8d9f-48d52dbe23a8)

Space Hunt features a title screen that displays the game name and background art. This screen is where settings can be adjusted and the game can be initiated or closed.

### Settings Screen
![image](https://github.com/user-attachments/assets/defa2e38-9a6c-46d6-b1e7-637b97ded8e0)

The settings screen features multiple buttons which allow the player to make changes to the game.

### In Game GUI
![image](https://github.com/user-attachments/assets/ce9a5b73-03db-4e33-8741-647b95497281)

The in game GUI displays the timer, the player's health, current score and the target score they must reach to proced to the next level.

## Artwork

### Arrow Sprite
![image](https://github.com/user-attachments/assets/ebf89051-64e8-46f5-909a-e676ddae1ca2)

The arrow sprite is used for the player's projectile attack. The arrow is shot when the player left clicks. When it hits an enemy, it deals damage to them. If it hits a wall, it despawns.
This projectile attack also has sound effects attached, for throwing the object, hitting a wolf, and hitting a wall.

Source: https://www.pixilart.com/art/game-o-games-arrow-sprite-667f585cd1cffae

### Coin Sprite
![image](https://github.com/user-attachments/assets/e209affc-f137-4b69-9cf1-6bba2d541bdd)

Coins spawn randomly throughout levels from level 2 onwards, and give the player 20 score. They produce a sound effect when picked up.

Source: https://iconduck.com/icons/313561/gold-coin

### Win Background
![image](https://github.com/user-attachments/assets/65c2a89a-6e7c-4439-9fa7-4990f8539e09)

This is the image used as the background of the scene displayed when the player completes the game.

Source: https://www.reddit.com/r/PixelArt/comments/f1wg26/space_background/
### Title Background
![image](https://github.com/user-attachments/assets/208d69c0-758f-48d5-98c0-73a913088d51)

Source: https://www.freepik.com/free-photos-vectors/space-background
## Textures and Sprites Derived from "Drop-Pod" GitHub Classroom Repo:
### Grass Textures
![image](https://github.com/user-attachments/assets/46e456d0-6449-4513-95b9-890783f8deee)

The first of these textures is used as the "floor" texture in the game.
### Wall Textures
![image](https://github.com/user-attachments/assets/d5e04e5b-7cda-413e-9b86-93e47cfffc97)

The first of these textures is used as the texture for "walls" in the game.
### Player Run & Idle Sprites
![image](https://github.com/user-attachments/assets/c9aaa7b3-4b26-48c3-a1d2-e74ad884d7c5)
![image](https://github.com/user-attachments/assets/1e2928ba-b7a0-4290-b948-bbc97d9e020f)

These are the textures used for the hunter while they are running (run sprites) or standing still (idle sprites).

### Monster Sprites v1 and v2
![image](https://github.com/user-attachments/assets/3b4a75b4-0fff-46dc-8e73-845034c50691)

![image](https://github.com/user-attachments/assets/36bcd731-a0e6-46b2-acea-ce8799bdcf25)

These sprites are used for the monsters in the game. From level 4 onwards, v2 is used, signifying the increasing danger of the late-game levels.

## Sound (sound effects)
The SoundManager class in the lib_sound library handles sound using SFML. It follows the singleton pattern, ensuring only one instance exists throughout the app. This instance manages both background music and sound effects.

For music, the "loadMusic" function loads a music file and stores it in a map with a name as the key. The "playMusic" function retrieves and plays the music, allowing you to set the volume and loop it. You can also stop, pause, or adjust the music's volume and looping settings. For sound effects, "loadSoundEffect" loads a sound file into a buffer and links it to a sound object. The "playSoundEffect" function plays the sound with the specified volume. If a sound or music isn't found, an error message is displayed.

### SFX
![image](https://github.com/user-attachments/assets/02ddc757-bb01-4a60-a36e-22d4bb65e948)

* Click.wav: SFX for when a menu button is clicked.
* Coin.wav: SFX for when a coin is picked up.

* Hit.wav: SFX for when an arrow hits an enemy.
* HitWall.wav: SFX for when an arrow hits a wall.
* Shoot.wav: SFX for shooting an arrow.

## Music
![image](https://github.com/user-attachments/assets/02ddc757-bb01-4a60-a36e-22d4bb65e948)

* Gameover.wav: Music for the Game Over event. Source: Undertale OST: "Determination" (https://www.youtube.com/watch?v=9kwKkGxF6_w)
* Level.wav: Music for the levels in the game. Source: VVVVVV OST: "Pushing Onwards" (https://www.youtube.com/watch?v=3Gj_88IMv3o)
* Title.wav: Title Screen music. Source: Metroid Prime: "Title Screen": (https://www.youtube.com/watch?v=42u0KB6f5eU)
* Win.wav: Win scene music. Source: Sonic Adventure 2; "Level Clear" OST: (https://www.youtube.com/watch?v=5HGUb9HXpOI)

Music was chosen for the game that would fit the space theme and pixel graphics.
VVVVVV and Undertale both being pixelated indie games meant their OSTs were good choices, and the Metroid Prime title screen music fit as a forebodeing soundtrack for the intro to the game.
The win screen music is intended to be jovial and celebratory, so a more light hearted tune was chosen for that track.

## Video Demo
[https://www.youtube.com/@liamflynn439](https://youtu.be/M4SjrTKm2xw)
