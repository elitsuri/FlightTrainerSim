// File: FlightMetrics.h
#ifndef FLIGHTMETRICS_H
#define FLIGHTMETRICS_H

#include "Aircraft.h"
#include "TrainingScenario.h"
#include <vector>
#include <string>
#include <map>

struct MetricSnapshot {
    double timestamp, altitude, speed, heading, verticalSpeed, deviationFromPath;
    bool stalled;
};

class FlightMetrics {
public:
    FlightMetrics();
    void reset();
    void recordSnapshot(const Aircraft& aircraft, double timestamp);
    void recordDeviation(const std::string& type, double value);
    const std::vector<MetricSnapshot>& snapshots() const { return m_snapshots; }
    const std::map<std::string, std::vector<double>>& deviations() const { return m_deviations; }
    double averageDeviation(const std::string& type) const;
    double maxDeviation(const std::string& type) const;
    int stallCount() const;
    double totalFlightTime() const;
private:
    std::vector<MetricSnapshot> m_snapshots;
    std::map<std::string, std::vector<double>> m_deviations;
    int m_stallCount;
};

class DebriefReport {
public:
    DebriefReport();
    void generate(const FlightMetrics& metrics, const TrainingScenario& scenario, const Aircraft& aircraft);
    double overallScore() const { return m_overallScore; }
    const std::string& summaryText() const { return m_summaryText; }
    const std::map<std::string, double>& categoryScores() const { return m_categoryScores; }
    const std::vector<std::string>& recommendations() const { return m_recommendations; }
private:
    double m_overallScore;
    std::string m_summaryText;
    std::map<std::string, double> m_categoryScores;
    std::vector<std::string> m_recommendations;
    double calculateAltitudeScore(const FlightMetrics& metrics);
    double calculateSpeedScore(const FlightMetrics& metrics);
    double calculateSmoothness(const FlightMetrics& metrics);
    double calculatePrecision(const FlightMetrics& metrics);
    void generateRecommendations();
};

#endif
