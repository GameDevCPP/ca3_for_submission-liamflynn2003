#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>
#include <system_renderer.h>
#include <system_resources.h>
#include <SFML/Audio.hpp>

enum button_states { BUTTON_IDLE, BUTTON_HOVER, BUTTON_ACTIVED, BUTTON_DOWN };

class Button : public Component {
private:
	sf::RectangleShape shape;
	std::shared_ptr<sf::Font> font;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;
public:
	sf::Text _text;

	short unsigned buttonState;
	sf::SoundBuffer sound_buffer;
	sf::Sound soundClick;

	Button() = delete;

	void update(float dt) override;
	void render() override;

	Button(Entity *p, sf::Vector2f position, const std::string &text, sf::Color idleColor, sf::Color hoverColor,
	       sf::Color activeColor);

	~Button() override = default;

	bool isPressed() const;

	static button_states _mouseState;
};
