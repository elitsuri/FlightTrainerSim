// File: SimulationEngine.h - WITH AUDIO
#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include "Aircraft.h"
#include "Environment.h"
#include "TrainingScenario.h"
#include "FlightMetrics.h"
#include "AircraftFactory.h"
#include "AudioSystem.h"
#include <QObject>
#include <QTimer>
#include <memory>

class SimulationEngine : public QObject {
    Q_OBJECT
public:
    explicit SimulationEngine(QObject* parent = nullptr);

    void start();
    void pause();
    void stop();
    void reset();

    bool isRunning() const { return m_isRunning; }
    bool isPaused() const { return m_isPaused; }

    Aircraft* activeAircraft() const { return m_activeAircraft.get(); }
    Environment* environment() const { return m_environment.get(); }
    TrainingScenario* scenario() const { return m_scenario.get(); }
    FlightMetrics* metrics() const { return m_metrics.get(); }
    AudioSystem* audio() const { return m_audioSystem.get(); }

    void setActiveAircraft(std::unique_ptr<Aircraft> aircraft);
    void setScenario(std::unique_ptr<TrainingScenario> scenario);
    void setControlInputs(const ControlInputs& controls);

    double simulationTime() const { return m_simulationTime; }

signals:
    void simulationUpdated();
    void scenarioFinished(bool success);
    void warningIssued(const QString& message);
    void stateChanged(const QString& state);

private slots:
    void updateSimulation();

private:
    std::unique_ptr<Aircraft> m_activeAircraft;
    std::unique_ptr<Environment> m_environment;
    std::unique_ptr<TrainingScenario> m_scenario;
    std::unique_ptr<FlightMetrics> m_metrics;
    std::unique_ptr<AudioSystem> m_audioSystem;
    std::unique_ptr<QTimer> m_updateTimer;

    bool m_isRunning, m_isPaused;
    double m_simulationTime;

    void checkWarnings();
    void updateAudio();
};

#endif