// File: Aircraft.cpp - COMPLETELY FIXED
#include "Aircraft.h"
#include "GlobalConfig.h"
#include <cmath>
#include <algorithm>

Aircraft::Aircraft(std::unique_ptr<IFlightModel> flightModel)
    : m_flightModel(std::move(flightModel)), m_fuel(1000.0), m_pathRecordTimer(0.0) {
    reset();
}

void Aircraft::reset() {
    // CRITICAL FIX: Start at proper altitude with speed
    m_position = { 0.0, 0.0, 100.0 };  // 100ft altitude
    m_orientation = { 0.0, 0.0, 0.0 };

    // CRITICAL: Initialize with forward velocity
    m_flightState = FlightState{};
    m_flightState.velocityX = 80.0;  // 80 knots forward speed
    m_flightState.velocityY = 0.0;
    m_flightState.velocityZ = 0.0;

    // Start with some throttle
    m_controls = ControlInputs{};
    m_controls.throttle = 0.5;  // 50% throttle
    m_controls.elevator = 0.0;
    m_controls.aileron = 0.0;
    m_controls.rudder = 0.0;
    m_controls.flaps = 0.0;
    m_controls.gearDown = true;

    m_fuel = 1000.0;
    m_flightPath.clear();
    m_pathRecordTimer = 0.0;
}

void Aircraft::update(double deltaTime) {
    if (m_fuel <= 0.0) {
        m_controls.throttle = 0.0;
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
    // Compute new forces
    FlightState newState;
    m_flightModel->computeForces(m_flightState, m_controls, altitude(), deltaTime, newState);

    // Update state
    m_flightState = newState;

    // Update position based on velocity
    m_position.x += m_flightState.velocityX * deltaTime;
    m_position.y += m_flightState.velocityY * deltaTime;
    m_position.z += m_flightState.velocityZ * deltaTime;

    // Ground collision
    if (m_position.z <= 0.0) {
        m_position.z = 0.0;

        // Stop vertical motion
        if (m_flightState.velocityZ < 0.0) {
            m_flightState.velocityZ = 0.0;
        }

        // Ground friction
        m_flightState.velocityX *= 0.95;
        m_flightState.velocityY *= 0.95;

        // If almost stopped, set to zero
        if (speed() < 5.0) {
            m_flightState.velocityX = 0.0;
            m_flightState.velocityY = 0.0;
        }
    }
}

void Aircraft::updateOrientation(double deltaTime) {
    // Update angles based on angular velocities
    m_orientation.bank += m_flightState.angularVelocityX;
    m_orientation.pitch += m_flightState.angularVelocityY;
    m_orientation.heading += m_flightState.angularVelocityZ;

    // Normalize heading (0-360)
    while (m_orientation.heading >= 360.0) m_orientation.heading -= 360.0;
    while (m_orientation.heading < 0.0) m_orientation.heading += 360.0;

    // Clamp pitch and bank
    m_orientation.pitch = std::clamp(m_orientation.pitch, -90.0, 90.0);
    m_orientation.bank = std::clamp(m_orientation.bank, -180.0, 180.0);

    // Natural damping for stability
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

    // Altitude limit
    if (m_position.z > config.maxAltitude()) {
        m_position.z = config.maxAltitude();
        m_flightState.velocityZ = std::min(0.0, m_flightState.velocityZ);
    }

    // Speed limit
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

double Aircraft::verticalSpeed() const {
    return m_flightState.velocityZ * 60.0; // Convert to feet per minute
}

bool Aircraft::isStalled() const {
    return speed() < m_flightModel->getStallSpeed() && altitude() > 10.0;
}

bool Aircraft::isOnGround() const {
    return m_position.z <= 1.0 && std::abs(m_flightState.velocityZ) < 1.0;
}

void Aircraft::recordPosition() {
    m_flightPath.push_back(m_position);
    if (m_flightPath.size() > 1000) {
        m_flightPath.erase(m_flightPath.begin());
    }
}