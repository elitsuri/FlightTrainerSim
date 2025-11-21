// File: AudioSystem.cpp
#include "AudioSystem.h"
#include <QUrl>
#include <QFile>

AudioSystem::AudioSystem(QObject* parent)
    : QObject(parent), m_enabled(false) {
    // Audio disabled by default - can be enabled if sound files available
    // initializeSounds();
}

void AudioSystem::initializeSounds() {
    // Engine sounds
    auto engineIdle = std::make_unique<QSoundEffect>();
    QString enginePath = getSoundPath(SoundType::EngineIdle);
    if (QFile::exists(enginePath)) {
        engineIdle->setSource(QUrl::fromLocalFile(enginePath));
        engineIdle->setLoopCount(QSoundEffect::Infinite);
        engineIdle->setVolume(0.3);
        m_sounds[SoundType::EngineIdle] = std::move(engineIdle);
        m_enabled = true;
    }

    // Warning sound
    auto warning = std::make_unique<QSoundEffect>();
    QString warningPath = getSoundPath(SoundType::Warning);
    if (QFile::exists(warningPath)) {
        warning->setSource(QUrl::fromLocalFile(warningPath));
        warning->setVolume(0.8);
        m_sounds[SoundType::Warning] = std::move(warning);
    }

    // Stall warning
    auto stall = std::make_unique<QSoundEffect>();
    QString stallPath = getSoundPath(SoundType::Stall);
    if (QFile::exists(stallPath)) {
        stall->setSource(QUrl::fromLocalFile(stallPath));
        stall->setLoopCount(QSoundEffect::Infinite);
        stall->setVolume(0.9);
        m_sounds[SoundType::Stall] = std::move(stall);
    }
}

QString AudioSystem::getSoundPath(SoundType type) const {
    QString basePath = "./sounds/";

    switch (type) {
    case SoundType::EngineIdle: return basePath + "engine_idle.wav";
    case SoundType::EngineHigh: return basePath + "engine_high.wav";
    case SoundType::Stall: return basePath + "stall_warning.wav";
    case SoundType::Warning: return basePath + "warning.wav";
    case SoundType::GearUp: return basePath + "gear_up.wav";
    case SoundType::GearDown: return basePath + "gear_down.wav";
    case SoundType::FlapsMove: return basePath + "flaps.wav";
    case SoundType::WindNoise: return basePath + "wind.wav";
    default: return "";
    }
}

void AudioSystem::playSound(SoundType type) {
    if (!m_enabled) return;

    auto it = m_sounds.find(type);
    if (it != m_sounds.end() && it->second) {
        it->second->play();
    }
}

void AudioSystem::stopSound(SoundType type) {
    if (!m_enabled) return;

    auto it = m_sounds.find(type);
    if (it != m_sounds.end() && it->second) {
        it->second->stop();
    }
}

void AudioSystem::setEngineVolume(double throttle) {
    if (!m_enabled) return;

    auto it = m_sounds.find(SoundType::EngineIdle);
    if (it != m_sounds.end() && it->second) {
        double volume = 0.3 + (throttle * 0.5); // 0.3 to 0.8
        it->second->setVolume(volume);

        if (!it->second->isPlaying()) {
            it->second->play();
        }
    }
}

void AudioSystem::setWindVolume(double speed) {
    if (!m_enabled) return;

    auto it = m_sounds.find(SoundType::WindNoise);
    if (it != m_sounds.end() && it->second) {
        double volume = std::min(speed / 200.0, 0.5); // Max 0.5 at 200+ knots
        it->second->setVolume(volume);
    }
}

void AudioSystem::playWarning() {
    playSound(SoundType::Warning);
}

void AudioSystem::stopAll() {
    for (auto& [type, sound] : m_sounds) {
        if (sound) {
            sound->stop();
        }
    }
}

void AudioSystem::setEnabled(bool enabled) {
    m_enabled = enabled;
    if (!enabled) {
        stopAll();
    }
}