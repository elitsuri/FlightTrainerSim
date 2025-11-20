// File: Environment.h
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <vector>
#include <optional>

struct Waypoint {
    double x, y, altitude;
    std::string name;
    double requiredSpeed = 0.0;
    double tolerance = 100.0;
};

struct Airfield {
    std::string name, icaoCode;
    double x, y, runwayHeading = 0.0, runwayLength = 2500.0;
};

struct WeatherCondition {
    std::string condition = "Clear";
    double windSpeed = 5.0, windDirection = 270.0, visibility = 10000.0, cloudBase = 3000.0;
};

class Environment {
public:
    Environment();
    void addAirfield(const Airfield& field);
    void addWaypoint(const Waypoint& wp);
    void setWeather(const WeatherCondition& weather);
    const std::vector<Airfield>& airfields() const { return m_airfields; }
    const std::vector<Waypoint>& waypoints() const { return m_waypoints; }
    const WeatherCondition& weather() const { return m_weather; }
    std::optional<Airfield> findNearestAirfield(double x, double y) const;
    std::optional<Waypoint> findWaypoint(const std::string& name) const;
    double distanceToPoint(double x1, double y1, double x2, double y2) const;
private:
    std::vector<Airfield> m_airfields;
    std::vector<Waypoint> m_waypoints;
    WeatherCondition m_weather;
};

#endif
