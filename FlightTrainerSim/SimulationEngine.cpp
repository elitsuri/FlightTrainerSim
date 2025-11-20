// File: SimulationEngine.cpp
#include "SimulationEngine.h"
#include "GlobalConfig.h"

SimulationEngine::SimulationEngine(QObject* parent)
    : QObject(parent), m_environment(std::make_unique<Environment>())
    , m_metrics(std::make_unique<FlightMetrics>()), m_updateTimer(std::make_unique<QTimer>(this))
    , m_isRunning(false), m_isPaused(false), m_simulationTime(0.0) {
    auto& config = GlobalConfig::instance();
    int updateIntervalMs = static_cast<int>(1000.0 / config.updateRateHz());
    m_updateTimer->setInterval(updateIntervalMs);
    connect(m_updateTimer.get(), &QTimer::timeout, this, &SimulationEngine::updateSimulation);
}

void SimulationEngine::start() {
    if (!m_activeAircraft || !m_scenario) return;
    m_isRunning = true;
    m_isPaused = false;
    m_updateTimer->start();
    emit stateChanged("Running");
}

void SimulationEngine::pause() {
    if (!m_isRunning) return;
    m_isPaused = !m_isPaused;
    if (m_isPaused) { m_updateTimer->stop(); emit stateChanged("Paused"); }
    else { m_updateTimer->start(); emit stateChanged("Running"); }
}

void SimulationEngine::stop() {
    m_isRunning = false;
    m_isPaused = false;
    m_updateTimer->stop();
    emit stateChanged("Stopped");
}

void SimulationEngine::reset() {
    stop();
    if (m_activeAircraft) m_activeAircraft->reset();
    if (m_scenario) m_scenario->reset();
    if (m_metrics) m_metrics->reset();
    m_simulationTime = 0.0;
    emit stateChanged("Reset");
    emit simulationUpdated();
}

void SimulationEngine::setActiveAircraft(std::unique_ptr<Aircraft> aircraft) {
    m_activeAircraft = std::move(aircraft);
}

void SimulationEngine::setScenario(std::unique_ptr<TrainingScenario> scenario) {
    m_scenario = std::move(scenario);
    if (m_scenario) m_scenario->reset();
}

void SimulationEngine::setControlInputs(const ControlInputs& controls) {
    if (m_activeAircraft) m_activeAircraft->setControls(controls);
}

void SimulationEngine::updateSimulation() {
    if (!m_activeAircraft || !m_scenario || m_isPaused) return;
    double deltaTime = GlobalConfig::instance().physicsTimeStep();
    m_activeAircraft->update(deltaTime);
    m_scenario->update(*m_activeAircraft, deltaTime);
    m_metrics->recordSnapshot(*m_activeAircraft, m_simulationTime);
    if (m_scenario->isCompleted()) { stop(); emit scenarioFinished(true); return; }
    else if (m_scenario->isFailed()) { stop(); emit scenarioFinished(false); return; }
    checkWarnings();
    m_simulationTime += deltaTime;
    emit simulationUpdated();
}

void SimulationEngine::checkWarnings() {
    if (!m_activeAircraft) return;
    if (m_activeAircraft->isStalled()) emit warningIssued("STALL WARNING");
    if (m_activeAircraft->fuel() < 100.0) emit warningIssued("LOW FUEL");
    if (m_activeAircraft->altitude() < 50.0 && !m_activeAircraft->isOnGround())
        emit warningIssued("ALTITUDE WARNING");
}
