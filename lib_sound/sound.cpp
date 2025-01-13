#include "sound.h"
#include <iostream>
#include <stdexcept>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::loadMusic(const std::string& name, const std::string& filepath) {
    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(filepath)) {
        throw std::runtime_error("Failed to open music file: " + filepath);
    }
    musicMap[name] = music;
}

void SoundManager::playMusic(const std::string& name, float volume, bool loop) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        auto& music = it->second;
        music->setVolume(volume);
        music->setLoop(loop);
        music->play();
    } else {
        std::cerr << "Music not found: " << name << std::endl;
    }
}

void SoundManager::stopMusic(const std::string& name) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        it->second->stop();
    }
}

void SoundManager::pauseMusic(const std::string& name) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        it->second->pause();
    }
}

sf::SoundSource::Status SoundManager::getMusicStatus(const std::string& name) const {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        return it->second->getStatus();
    }
    return sf::SoundSource::Stopped;
}

void SoundManager::setMusicVolume(const std::string& name, float volume) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        it->second->setVolume(volume);
    } else {
        std::cerr << "Music not found: " << name << std::endl;
    }
}

void SoundManager::setMusicLoop(const std::string& name, bool loop) {
    auto it = musicMap.find(name);
    if (it != musicMap.end()) {
        it->second->setLoop(loop);
    } else {
        std::cerr << "Music not found: " << name << std::endl;
    }
}

void SoundManager::loadSoundEffect(const std::string& name, const std::string& filepath) {
    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filepath)) {
        throw std::runtime_error("Failed to load sound effect: " + filepath);
    }
    soundEffectBuffers[name] = buffer;

    auto sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*buffer);
    soundEffects[name] = sound;
}

void SoundManager::playSoundEffect(const std::string& name, float volume) {
    auto it = soundEffects.find(name);
    if (it != soundEffects.end()) {
        auto& sound = it->second;
        sound->setVolume(volume);
        sound->play();
    } else {
        std::cerr << "Sound effect not found: " << name << std::endl;
    }
}

