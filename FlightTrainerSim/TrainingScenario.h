// File: TrainingScenario.h
#ifndef TRAININGSCENARIO_H
#define TRAININGSCENARIO_H

#include "Environment.h"
#include "Aircraft.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

enum class ScenarioState {
    PreFlight, Takeoff, Climb, Cruise, Approach, Landing, Completed, Failed
};

struct StateTransitionRule {
    ScenarioState fromState, toState;
    std::function<bool(const Aircraft&)> condition;
    std::string description;
};

class TrainingScenario {
public:
    TrainingScenario(const std::string& name, const std::string& description);
    void reset();
    void update(const Aircraft& aircraft, double deltaTime);
    const std::string& name() const { return m_name; }
    const std::string& description() const { return m_description; }
    ScenarioState currentState() const { return m_currentState; }
    const std::vector<Waypoint>& targetWaypoints() const { return m_targetWaypoints; }
    bool isCompleted() const { return m_currentState == ScenarioState::Completed; }
    bool isFailed() const { return m_currentState == ScenarioState::Failed; }
    void addWaypoint(const Waypoint& wp) { m_targetWaypoints.push_back(wp); }
    void addTransitionRule(const StateTransitionRule& rule) { m_transitionRules.push_back(rule); }
    std::string getCurrentStateDescription() const;
    double getProgress() const { return m_progress; }
    const std::vector<std::string>& getMessages() const { return m_messages; }
    static std::unique_ptr<TrainingScenario> createBasicTakeoffScenario();
    static std::unique_ptr<TrainingScenario> createPatternScenario();
    static std::unique_ptr<TrainingScenario> createIFRBasicScenario();
    static std::unique_ptr<TrainingScenario> createEngineFailureScenario();
private:
    std::string m_name, m_description;
    ScenarioState m_currentState;
    std::vector<Waypoint> m_targetWaypoints;
    std::vector<StateTransitionRule> m_transitionRules;
    std::vector<std::string> m_messages;
    double m_progress, m_stateTimer;
    size_t m_currentWaypointIndex;
    void transitionTo(ScenarioState newState);
    void addMessage(const std::string& msg);
    void checkTransitions(const Aircraft& aircraft);
    void updateProgress(const Aircraft& aircraft);
};

#endif
