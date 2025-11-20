// File: FlightMetrics.cpp
#include "FlightMetrics.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>

FlightMetrics::FlightMetrics() : m_stallCount(0) {}
void FlightMetrics::reset() { m_snapshots.clear(); m_deviations.clear(); m_stallCount = 0; }

void FlightMetrics::recordSnapshot(const Aircraft& aircraft, double timestamp) {
    MetricSnapshot snap;
    snap.timestamp = timestamp;
    snap.altitude = aircraft.altitude();
    snap.speed = aircraft.speed();
    snap.heading = aircraft.heading();
    snap.verticalSpeed = aircraft.verticalSpeed();
    snap.deviationFromPath = 0.0;
    snap.stalled = aircraft.isStalled();
    if (snap.stalled) m_stallCount++;
    m_snapshots.push_back(snap);
}

void FlightMetrics::recordDeviation(const std::string& type, double value) {
    m_deviations[type].push_back(value);
}

double FlightMetrics::averageDeviation(const std::string& type) const {
    auto it = m_deviations.find(type);
    if (it == m_deviations.end() || it->second.empty()) return 0.0;
    return std::accumulate(it->second.begin(), it->second.end(), 0.0) / it->second.size();
}

double FlightMetrics::maxDeviation(const std::string& type) const {
    auto it = m_deviations.find(type);
    if (it == m_deviations.end() || it->second.empty()) return 0.0;
    return *std::max_element(it->second.begin(), it->second.end());
}

int FlightMetrics::stallCount() const { return m_stallCount; }

double FlightMetrics::totalFlightTime() const {
    if (m_snapshots.empty()) return 0.0;
    return m_snapshots.back().timestamp - m_snapshots.front().timestamp;
}

DebriefReport::DebriefReport() : m_overallScore(0.0) {}

void DebriefReport::generate(const FlightMetrics& metrics, const TrainingScenario& scenario, const Aircraft& aircraft) {
    m_categoryScores.clear();
    m_recommendations.clear();
    m_categoryScores["Altitude Control"] = calculateAltitudeScore(metrics);
    m_categoryScores["Speed Control"] = calculateSpeedScore(metrics);
    m_categoryScores["Smoothness"] = calculateSmoothness(metrics);
    m_categoryScores["Precision"] = calculatePrecision(metrics);
    if (scenario.isCompleted()) m_categoryScores["Scenario Completion"] = 100.0;
    else if (scenario.isFailed()) m_categoryScores["Scenario Completion"] = 0.0;
    else m_categoryScores["Scenario Completion"] = scenario.getProgress();
    m_overallScore = 0.0;
    for (const auto& [category, score] : m_categoryScores) m_overallScore += score;
    m_overallScore /= m_categoryScores.size();
    std::ostringstream oss;
    oss << "Flight Training Debrief\n=======================\n\n";
    oss << "Scenario: " << scenario.name() << "\n";
    oss << "Aircraft: " << aircraft.flightModel()->getModelName() << "\n";
    oss << "Flight Time: " << static_cast<int>(metrics.totalFlightTime()) << " seconds\n\n";
    oss << "Overall Score: " << static_cast<int>(m_overallScore) << "/100\n\n";
    oss << "Category Breakdown:\n";
    for (const auto& [category, score] : m_categoryScores) {
        oss << "  " << category << ": " << static_cast<int>(score) << "/100\n";
    }
    m_summaryText = oss.str();
    generateRecommendations();
}

double DebriefReport::calculateAltitudeScore(const FlightMetrics& metrics) {
    double avgDev = metrics.averageDeviation("altitude");
    if (avgDev < 50.0) return 100.0;
    if (avgDev < 100.0) return 90.0;
    if (avgDev < 200.0) return 75.0;
    if (avgDev < 300.0) return 60.0;
    return 40.0;
}

double DebriefReport::calculateSpeedScore(const FlightMetrics& metrics) {
    double avgDev = metrics.averageDeviation("speed");
    if (metrics.stallCount() > 0) return 30.0;
    if (avgDev < 5.0) return 100.0;
    if (avgDev < 10.0) return 85.0;
    if (avgDev < 20.0) return 70.0;
    return 50.0;
}

double DebriefReport::calculateSmoothness(const FlightMetrics& metrics) {
    const auto& snaps = metrics.snapshots();
    if (snaps.size() < 2) return 100.0;
    double totalJerk = 0.0;
    for (size_t i = 1; i < snaps.size(); ++i) {
        double dVS = std::abs(snaps[i].verticalSpeed - snaps[i-1].verticalSpeed);
        totalJerk += dVS;
    }
    double avgJerk = totalJerk / (snaps.size() - 1);
    if (avgJerk < 5.0) return 100.0;
    if (avgJerk < 10.0) return 85.0;
    if (avgJerk < 20.0) return 70.0;
    return 55.0;
}

double DebriefReport::calculatePrecision(const FlightMetrics& metrics) {
    double avgDev = metrics.averageDeviation("heading");
    if (avgDev < 2.0) return 100.0;
    if (avgDev < 5.0) return 90.0;
    if (avgDev < 10.0) return 75.0;
    return 60.0;
}

void DebriefReport::generateRecommendations() {
    if (m_categoryScores["Altitude Control"] < 70.0)
        m_recommendations.push_back("Focus on maintaining constant altitude during cruise.");
    if (m_categoryScores["Speed Control"] < 70.0)
        m_recommendations.push_back("Practice throttle management for better speed control.");
    if (m_categoryScores["Smoothness"] < 70.0)
        m_recommendations.push_back("Make smoother control inputs to avoid abrupt maneuvers.");
    if (m_categoryScores["Precision"] < 70.0)
        m_recommendations.push_back("Improve heading precision during navigation segments.");
    if (m_overallScore >= 90.0)
        m_recommendations.push_back("Excellent performance! Ready for advanced scenarios.");
}
