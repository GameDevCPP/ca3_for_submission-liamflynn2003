
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>

class SoundManager {
public:
    static SoundManager& getInstance();

    // MUSIC
    void loadMusic(const std::string& name, const std::string& filepath);
    void playMusic(const std::string& name, float volume = 100.f, bool loop = true);
    void stopMusic(const std::string& name);
    void pauseMusic(const std::string& name);
    sf::SoundSource::Status getMusicStatus(const std::string& name) const;

    void setMusicVolume(const std::string &name, float volume);

    void setMusicLoop(const std::string &name, bool loop);

    // SFX
    void loadSoundEffect(const std::string& name, const std::string& filepath);
    void playSoundEffect(const std::string& name, float volume = 100.f);
private:
    SoundManager() = default;
    std::map<std::string, std::shared_ptr<sf::Music>> musicMap;
    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundEffectBuffers;
    std::unordered_map<std::string, std::shared_ptr<sf::Sound>> soundEffects;
};

