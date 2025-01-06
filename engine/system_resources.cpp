#include "system_resources.h"

namespace Resources {
	template<>
	std::shared_ptr<sf::Font> load(const std::string& name) {
        auto font = std::make_shared<sf::Font>();
        if (!font->loadFromFile("res/fonts/" + name)) {
            throw std::runtime_error("Failed to load font: " + name);
        }
		return font;
	};

	template <> // explicit specialization for T = texture
	std::shared_ptr<sf::Texture> load(const std::string& name) {
		auto tex = std::make_shared<sf::Texture>();
        if (!tex->loadFromFile("res/img/" + name)) {
            throw std::runtime_error("Failed to load texture: " + name);
        }
		return tex;
	};
	template <> // explicit specialization for T = SoundBuffer
	std::shared_ptr<sf::SoundBuffer> load(const std::string& name) {
		auto buf = std::make_shared<sf::SoundBuffer>();
		if (!buf->loadFromFile("res/sound/" + name)) {
			throw std::runtime_error("Failed to load sound buffer: " + name);
		}
		return buf;
	};

	template <> // explicit specialization for T = Music
	std::shared_ptr<sf::Music> load(const std::string& name) {
		auto music = std::make_shared<sf::Music>();
		if (!music->openFromFile("res/sound/" + name)) {
			throw std::runtime_error("Failed to load music file: " + name);
		}
		return music;
	}

}
