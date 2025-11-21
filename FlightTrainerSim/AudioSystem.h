// File: AudioSystem.h - NEW AUDIO SYSTEM
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <QObject>
#include <QSoundEffect>
#include <memory>
#include <map>

enum class SoundType {
    EngineIdle,
    EngineHigh,
    Stall,
    Warning,
    GearUp,
    GearDown,
    FlapsMove,
    WindNoise
};

class AudioSystem : public QObject {
    Q_OBJECT
public:
    explicit AudioSystem(QObject* parent = nullptr);
    ~AudioSystem() = default;

    void playSound(SoundType type);
    void stopSound(SoundType type);
    void setEngineVolume(double throttle); // 0.0 to 1.0
    void setWindVolume(double speed);      // Based on airspeed
    void playWarning();
    void stopAll();

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled);

private:
    bool m_enabled;
    std::map<SoundType, std::unique_ptr<QSoundEffect>> m_sounds;

    void initializeSounds();
    QString getSoundPath(SoundType type) const;
};

#endif