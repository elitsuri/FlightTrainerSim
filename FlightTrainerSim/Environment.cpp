// File: Environment.cpp
#include "Environment.h"
#include <cmath>
#include <algorithm>
#include <limits>

Environment::Environment() {
    addAirfield({"Home Base", "KHBF", 0.0, 0.0, 90.0, 3000.0});
    addAirfield({"Training Field", "KTRN", 50000.0, 25000.0, 180.0, 2500.0});
    addWaypoint({10000.0, 0.0, 1000.0, "Alpha", 150.0, 200.0});
    addWaypoint({20000.0, 10000.0, 2000.0, "Bravo", 180.0, 200.0});
    addWaypoint({30000.0, 5000.0, 1500.0, "Charlie", 160.0, 200.0});
    m_weather = {"Clear", 5.0, 270.0, 10000.0, 3000.0};
}

void Environment::addAirfield(const Airfield& field) { m_airfields.push_back(field); }
void Environment::addWaypoint(const Waypoint& wp) { m_waypoints.push_back(wp); }
void Environment::setWeather(const WeatherCondition& weather) { m_weather = weather; }

std::optional<Airfield> Environment::findNearestAirfield(double x, double y) const {
    if (m_airfields.empty()) return std::nullopt;
    double minDist = std::numeric_limits<double>::max();
    size_t nearestIdx = 0;
    for (size_t i = 0; i < m_airfields.size(); ++i) {
        double dist = distanceToPoint(x, y, m_airfields[i].x, m_airfields[i].y);
        if (dist < minDist) { minDist = dist; nearestIdx = i; }
    }
    return m_airfields[nearestIdx];
}

std::optional<Waypoint> Environment::findWaypoint(const std::string& name) const {
    auto it = std::find_if(m_waypoints.begin(), m_waypoints.end(),
                          [&name](const Waypoint& wp) { return wp.name == name; });
    if (it != m_waypoints.end()) return *it;
    return std::nullopt;
}

double Environment::distanceToPoint(double x1, double y1, double x2, double y2) const {
    double dx = x2 - x1, dy = y2 - y1;
    return std::sqrt(dx*dx + dy*dy);
}
