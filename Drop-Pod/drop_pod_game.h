#pragma once

#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_planet_level.h"
#include "scenes/scene_planet_level2.h"

extern MenuScene menu;
extern SettingsScene settings;
extern PlanetLevelScene planetLevel;
extern PlanetLevel2Scene planetLevel2;
extern short unsigned switchState;
extern short unsigned switchRes;
extern int volume;
extern sf::Vector2i resolution;