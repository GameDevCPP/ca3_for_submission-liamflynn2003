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
In a world where humanity explores the stars, you play as a huntter, whose ship has crash landed on a strange planet. Dedicated to your duties, you resolve to make the most of your unwanted trip - by hunting the local fauna and collecting treasure as you survive the hostile planet and attempt escape.

Having ejected from his ship, the hunter is only left with rudimentary arrow weapons which he crafted. With these arrows as his only weapons, and with monster alien wolves hunting him at every step, the hunter begins his journey.

## Objective
The player must increase their score by hunting wolves and collecting coins on each level.
Once a certain score is reached, the player will proceed to the next level, which varies in visual design and layout.
Each level increases in difficulty, and once the player completes the fifth and fiinal level, the hunt is complete, the hunter escapes the planet and the player views a win screen.

## Gameplay
Players spawn on a set spawn tile in each level. Level One features no coins, but 4 enemies will spawn and run towards the player.
The player character, the Hunter, has 100 hp, and each enemy attack reduces it by 2 hp - once an enemy reaches the player, they hunter, they will attack.
Each wolf kill provides the player with 10 score, and each coin they collect provides the player with 20 score, which is displayed in the HUD.
Wolves spawn from "enemy spawn" tiles, which are location in certain locations in each level. Once spawned, they will begin to move towards the player.
Wolves spawn every 20 seconds.
Levels become increasingly more mazelike as the game progresses, making coins harder to find, and making it easier to be caught by the wolves and pinned in a dead-end / corner.
Wolves will move around walls if blocked access to the player.
Coins spawn at random locations in each level, from Level 2 onwards. They cannot spawn too close to each other, and can only spawn on floor tiles.

## Levels
### Level 1
On level 1, there are 4 enemy spawners, which spawn enemy wolves every 20 seconds. The player must reach 100 score on Level 1 to proceed to Level 2.
The player spawns in the middle of the level, and it does not feature many walls, allowing the player to learn the mechanics on an easy level.

### Level 2
Level 2 introduces coins, which can be collected to increase a player's score by 20. This level is vertically larger than level one but the same width, and features 6 enemy spawn points.
There are blocks of walls around the level, and the player spawns in the centre. As the player discovers coins, they will learn that exploring levels to search for coins will help to complete levels faster, before too many wolves spawn.

### Level 3
Level 3 is smaller veretically and larger horizontally then the previous levels.
It features 8 olf spawn points, and spawns the player at one side of the map, which encourages the player to move deeper into the level to hunt for more wolves and coins.

### Level 4
Level 4 is a mazelike level, which is vertically large and spawns the player at the bottom. The player has to move up through the level to find coins and wolves, and it features corners and deadends within the maze, which the player could get trapped in if they navigate poorly and are chased by wolves. the level features 7 wolf spawn points.

### Level 5
Level 5 is a series of tight mazelike corridors, with many wolf spawn points. Players will have to dodge wolves in these tighht spaces while collecting coins and defeating the wolves to increase their score.
Like the last level, the player will have to navigate through the level towards more enemy spawns to collet coins, so they can complete the level before getting overwhelmed. It serves as a final challenge before completing the game.

## Controls
Players use WASD to move, and left click to shoot. This player has the option to switch to using arrow keys for movement in the settings menu.

## GUI/In-game menu
### Splash Screen + Menu

### In Game GUI

## Artwork
* Describe the artwork used and source/origin of the artwork
* Describe how you implemented and managed the textures (was memory management techniques used)


## Sound (sound effects)
* Describe how sound is implemented and managed using SFML
* Describe what sound effects your game has


## Music
* Describe the music used and its source/origin
* Is there a menu track, and a track for main game (level).
* Is the music randomly chosen for each game.
