#include "engine.h"
#include "space_hunt_game.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_win.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

MenuScene menu;
SettingsScene settings;
PlanetLevelScene planetLevel;
WinScene win;

short unsigned switchState;
float volume;
Vector2i resolution;

int main() {
	resolution = Vector2i(1280, 720);
	volume = 20;
	Engine::Start(resolution.x, resolution.y, "Space Hunt", &menu);
}