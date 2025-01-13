#pragma once

#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_planet_level.h"
#include "scenes/scene_win.h"

extern MenuScene menu;
extern SettingsScene settings;
extern PlanetLevelScene planetLevel;
extern WinScene win;
extern short unsigned switchState;
extern short unsigned switchRes;
extern float volume;
extern sf::Vector2i resolution;