// File: TrainingScenario.cpp
#include "TrainingScenario.h"
#include <cmath>

TrainingScenario::TrainingScenario(const std::string& name, const std::string& description)
    : m_name(name), m_description(description), m_currentState(ScenarioState::PreFlight)
    , m_progress(0.0), m_stateTimer(0.0), m_currentWaypointIndex(0) {}

void TrainingScenario::reset() {
    m_currentState = ScenarioState::PreFlight;
    m_progress = 0.0;
    m_stateTimer = 0.0;
    m_currentWaypointIndex = 0;
    m_messages.clear();
    addMessage("Scenario initialized. Ready for pre-flight checks.");
}

void TrainingScenario::update(const Aircraft& aircraft, double deltaTime) {
    m_stateTimer += deltaTime;
    checkTransitions(aircraft);
    updateProgress(aircraft);
}

void TrainingScenario::transitionTo(ScenarioState newState) {
    m_currentState = newState;
    m_stateTimer = 0.0;
    addMessage("State changed to: " + getCurrentStateDescription());
}

void TrainingScenario::addMessage(const std::string& msg) {
    m_messages.push_back(msg);
    if (m_messages.size() > 50) m_messages.erase(m_messages.begin());
}

void TrainingScenario::checkTransitions(const Aircraft& aircraft) {
    for (const auto& rule : m_transitionRules) {
        if (rule.fromState == m_currentState && rule.condition(aircraft)) {
            transitionTo(rule.toState);
            addMessage(rule.description);
            break;
        }
    }
}

void TrainingScenario::updateProgress(const Aircraft& aircraft) {
    if (m_targetWaypoints.empty()) return;
    const auto& currentWP = m_targetWaypoints[m_currentWaypointIndex];
    double dx = aircraft.position().x - currentWP.x;
    double dy = aircraft.position().y - currentWP.y;
    double dz = aircraft.altitude() - currentWP.altitude;
    double dist = std::sqrt(dx*dx + dy*dy + dz*dz);
    if (dist < currentWP.tolerance) {
        m_currentWaypointIndex++;
        if (m_currentWaypointIndex >= m_targetWaypoints.size()) {
            m_currentWaypointIndex = m_targetWaypoints.size() - 1;
            m_progress = 100.0;
        } else {
            m_progress = (100.0 * m_currentWaypointIndex) / m_targetWaypoints.size();
            addMessage("Waypoint reached: " + currentWP.name);
        }
    }
}

std::string TrainingScenario::getCurrentStateDescription() const {
    switch (m_currentState) {
        case ScenarioState::PreFlight: return "Pre-Flight Check";
        case ScenarioState::Takeoff: return "Takeoff Roll";
        case ScenarioState::Climb: return "Initial Climb";
        case ScenarioState::Cruise: return "Cruise Flight";
        case ScenarioState::Approach: return "Approach";
        case ScenarioState::Landing: return "Landing";
        case ScenarioState::Completed: return "Completed Successfully";
        case ScenarioState::Failed: return "Failed";
        default: return "Unknown";
    }
}

std::unique_ptr<TrainingScenario> TrainingScenario::createBasicTakeoffScenario() {
    auto scenario = std::make_unique<TrainingScenario>("Basic Takeoff", "Practice standard takeoff procedure");
    scenario->addWaypoint({0, 0, 0, "Runway Start", 0, 50});
    scenario->addWaypoint({3000, 0, 300, "Rotation Point", 80, 100});
    scenario->addWaypoint({6000, 0, 1000, "Pattern Altitude", 120, 150});
    StateTransitionRule r1;
    r1.fromState = ScenarioState::PreFlight; r1.toState = ScenarioState::Takeoff;
    r1.condition = [](const Aircraft& a) { return a.controls().throttle > 0.7 && a.isOnGround(); };
    r1.description = "Takeoff roll initiated";
    scenario->addTransitionRule(r1);
    StateTransitionRule r2;
    r2.fromState = ScenarioState::Takeoff; r2.toState = ScenarioState::Climb;
    r2.condition = [](const Aircraft& a) { return a.altitude() > 50 && a.speed() > 70; };
    r2.description = "Positive rate of climb";
    scenario->addTransitionRule(r2);
    StateTransitionRule r3;
    r3.fromState = ScenarioState::Climb; r3.toState = ScenarioState::Completed;
    r3.condition = [](const Aircraft& a) { return a.altitude() > 900; };
    r3.description = "Pattern altitude achieved";
    scenario->addTransitionRule(r3);
    return scenario;
}

std::unique_ptr<TrainingScenario> TrainingScenario::createPatternScenario() {
    auto scenario = std::make_unique<TrainingScenario>("Traffic Pattern", "Complete standard traffic pattern");
    scenario->addWaypoint({0, 0, 1000, "Upwind", 100, 200});
    scenario->addWaypoint({5000, 3000, 1000, "Crosswind", 90, 200});
    scenario->addWaypoint({5000, 8000, 1000, "Downwind", 90, 200});
    scenario->addWaypoint({2000, 8000, 500, "Base", 80, 200});
    scenario->addWaypoint({0, 3000, 100, "Final", 70, 150});
    scenario->addWaypoint({0, 0, 0, "Touchdown", 60, 100});
    return scenario;
}

std::unique_ptr<TrainingScenario> TrainingScenario::createIFRBasicScenario() {
    auto scenario = std::make_unique<TrainingScenario>("IFR Basic Navigation", "Follow instrument procedures");
    scenario->addWaypoint({10000, 0, 3000, "VOR Alpha", 150, 300});
    scenario->addWaypoint({20000, 10000, 5000, "VOR Bravo", 180, 300});
    scenario->addWaypoint({30000, 5000, 3000, "VOR Charlie", 160, 300});
    scenario->addWaypoint({40000, 0, 1000, "Initial Approach Fix", 140, 250});
    return scenario;
}

std::unique_ptr<TrainingScenario> TrainingScenario::createEngineFailureScenario() {
    auto scenario = std::make_unique<TrainingScenario>("Engine Failure Recovery", "Handle engine failure at cruise");
    scenario->addWaypoint({5000, 0, 3000, "Cruise Point", 180, 300});
    scenario->addWaypoint({8000, 2000, 2000, "Emergency Descent", 120, 300});
    scenario->addWaypoint({10000, 3000, 500, "Emergency Approach", 90, 250});
    scenario->addWaypoint({12000, 3500, 0, "Emergency Landing", 70, 150});
    return scenario;
}
