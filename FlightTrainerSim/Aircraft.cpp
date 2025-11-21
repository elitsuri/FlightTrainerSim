// File: Aircraft.cpp - FIXED VERSION
#include "Aircraft.h"
#include "GlobalConfig.h"
#include <cmath>
#include <algorithm>

Aircraft::Aircraft(std::unique_ptr<IFlightModel> flightModel)
    : m_flightModel(std::move(flightModel)), m_fuel(1000.0), m_pathRecordTimer(0.0) {
    reset();
}

void Aircraft::reset() {
    // FIX 1: Start at altitude with forward velocity
    m_position = { 0.0, 0.0, 100.0 };  // Start at 100ft altitude!
    m_orientation = { 0.0, 0.0, 0.0 };
    m_flightState = FlightState{};
    m_flightState.velocityX = 50.0;  // Start with 50 knots forward
    m_controls = ControlInputs{};
    m_controls.throttle = 0.3;       // Start with some throttle
    m_fuel = 1000.0;
    m_flightPath.clear();
    m_pathRecordTimer = 0.0;
}

void Aircraft::update(double deltaTime) {
    if (m_fuel <= 0.0) {
        m_controls.throttle = 0.0; // Engine out but keep updating
    }

    updatePhysics(deltaTime);
    updateOrientation(deltaTime);
    updateFuel(deltaTime);
    enforceConstraints();

    m_pathRecordTimer += deltaTime;
    if (m_pathRecordTimer >= 0.5) {
        recordPosition();
        m_pathRecordTimer = 0.0;
    }
}

void Aircraft::updatePhysics(double deltaTime) {
    FlightState newState;
    m_flightModel->computeForces(m_flightState, m_controls, altitude(), deltaTime, newState);

    m_flightState = newState;

    // Update position
    m_position.x += m_flightState.velocityX * deltaTime;
    m_position.y += m_flightState.velocityY * deltaTime;
    m_position.z += m_flightState.velocityZ * deltaTime;

    // FIX 2: Better ground collision
    if (m_position.z < 0.0) {
        m_position.z = 0.0;
        m_flightState.velocityZ = std::max(0.0, m_flightState.velocityZ);
        m_flightState.velocityX *= 0.95; // Ground friction
    }
}

void Aircraft::updateOrientation(double deltaTime) {
    m_orientation.bank += m_flightState.angularVelocityX;
    m_orientation.pitch += m_flightState.angularVelocityY;
    m_orientation.heading += m_flightState.angularVelocityZ;

    // Normalize angles
    while (m_orientation.heading >= 360.0) m_orientation.heading -= 360.0;
    while (m_orientation.heading < 0.0) m_orientation.heading += 360.0;

    m_orientation.pitch = std::clamp(m_orientation.pitch, -90.0, 90.0);
    m_orientation.bank = std::clamp(m_orientation.bank, -180.0, 180.0);

    // FIX 3: Add damping for stability
    m_orientation.bank *= 0.98;
    m_orientation.pitch *= 0.98;
}

void Aircraft::updateFuel(double deltaTime) {
    double consumption = m_flightModel->getFuelConsumptionRate() * m_controls.throttle;
    m_fuel -= consumption * deltaTime;
    if (m_fuel < 0.0) m_fuel = 0.0;
}

void Aircraft::enforceConstraints() {
    auto& config = GlobalConfig::instance();

    if (m_position.z > config.maxAltitude()) {
        m_position.z = config.maxAltitude();
        m_flightState.velocityZ = std::min(0.0, m_flightState.velocityZ);
    }

    double currentSpeed = speed();
    if (currentSpeed > config.maxSpeed()) {
        double scale = config.maxSpeed() / currentSpeed;
        m_flightState.velocityX *= scale;
        m_flightState.velocityY *= scale;
        m_flightState.velocityZ *= scale;
    }
}

double Aircraft::speed() const {
    return std::sqrt(m_flightState.velocityX * m_flightState.velocityX +
        m_flightState.velocityY * m_flightState.velocityY +
        m_flightState.velocityZ * m_flightState.velocityZ);
}

bool Aircraft::isStalled() const {
    return speed() < m_flightModel->getStallSpeed() && altitude() > 10.0;
}

void Aircraft::recordPosition() {
    m_flightPath.push_back(m_position);
    if (m_flightPath.size() > 1000) {
        m_flightPath.erase(m_flightPath.begin());
    }
}