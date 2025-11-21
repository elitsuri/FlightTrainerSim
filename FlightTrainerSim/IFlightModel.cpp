// File: IFlightModel.cpp - COMPLETELY FIXED
#include "IFlightModel.h"
#include "GlobalConfig.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// TrainerFlightModel - T-38 Trainer
// ============================================================================
void TrainerFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
    double altitude, double deltaTime, FlightState& outNewState) {
    const double mass = 5500.0;
    const double wingArea = 15.8;
    const double dragCoeff = 0.025;
    const double liftCoeff = 5.5;
    const double airDensity = 1.225 * std::exp(-altitude / 10000.0);

    // Current speed
    double speed = std::sqrt(state.velocityX * state.velocityX +
        state.velocityY * state.velocityY +
        state.velocityZ * state.velocityZ);
    if (speed < 1.0) speed = 1.0; // Prevent division by zero

    // Dynamic pressure
    double q = 0.5 * airDensity * speed * speed;

    // Forces
    double thrust = controls.throttle * getMaxThrust();
    double drag = q * wingArea * (dragCoeff + controls.flaps * 0.1);
    double lift = q * wingArea * (liftCoeff + controls.flaps * 1.5) * (1.0 + controls.elevator * 0.3);
    double weight = mass * GlobalConfig::instance().gravity();

    // Accelerations
    double accelX = (thrust - drag) / mass;
    double accelY = (controls.aileron * 2.0) * q / mass;
    double accelZ = (lift - weight) / mass + (controls.elevator * 15.0);

    // Update velocities
    outNewState.velocityX = state.velocityX + accelX * deltaTime;
    outNewState.velocityY = state.velocityY + accelY * deltaTime;
    outNewState.velocityZ = state.velocityZ + accelZ * deltaTime;

    // Angular velocities (rotation rates)
    outNewState.angularVelocityX = controls.aileron * 30.0 * deltaTime;  // Bank rate
    outNewState.angularVelocityY = controls.elevator * 20.0 * deltaTime;  // Pitch rate
    outNewState.angularVelocityZ = controls.rudder * 15.0 * deltaTime;    // Yaw rate

    outNewState.thrust = thrust;
    outNewState.drag = drag;
    outNewState.lift = lift;
}

// ============================================================================
// JetFlightModel - F-16 Fighting Falcon
// ============================================================================
void JetFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
    double altitude, double deltaTime, FlightState& outNewState) {
    const double mass = 12000.0;
    const double wingArea = 27.87;
    const double dragCoeff = 0.018;
    const double liftCoeff = 6.2;
    const double airDensity = 1.225 * std::exp(-altitude / 10000.0);

    double speed = std::sqrt(state.velocityX * state.velocityX +
        state.velocityY * state.velocityY +
        state.velocityZ * state.velocityZ);
    if (speed < 1.0) speed = 1.0;

    double q = 0.5 * airDensity * speed * speed;

    // Afterburner boost
    double thrustMultiplier = (controls.throttle > 0.9) ? 1.5 : 1.0;
    double thrust = controls.throttle * getMaxThrust() * thrustMultiplier;
    double drag = q * wingArea * (dragCoeff + controls.flaps * 0.08);
    double lift = q * wingArea * (liftCoeff + controls.flaps * 1.2) * (1.0 + controls.elevator * 0.25);
    double weight = mass * GlobalConfig::instance().gravity();

    double accelX = (thrust - drag) / mass;
    double accelY = (controls.aileron * 2.5) * q / mass;
    double accelZ = (lift - weight) / mass + (controls.elevator * 20.0);

    outNewState.velocityX = state.velocityX + accelX * deltaTime;
    outNewState.velocityY = state.velocityY + accelY * deltaTime;
    outNewState.velocityZ = state.velocityZ + accelZ * deltaTime;

    outNewState.angularVelocityX = controls.aileron * 45.0 * deltaTime;
    outNewState.angularVelocityY = controls.elevator * 30.0 * deltaTime;
    outNewState.angularVelocityZ = controls.rudder * 20.0 * deltaTime;

    outNewState.thrust = thrust;
    outNewState.drag = drag;
    outNewState.lift = lift;
}

// ============================================================================
// CargoFlightModel - C-130 Hercules
// ============================================================================
void CargoFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
    double altitude, double deltaTime, FlightState& outNewState) {
    const double mass = 70000.0;
    const double wingArea = 162.1;
    const double dragCoeff = 0.035;
    const double liftCoeff = 5.0;
    const double airDensity = 1.225 * std::exp(-altitude / 10000.0);

    double speed = std::sqrt(state.velocityX * state.velocityX +
        state.velocityY * state.velocityY +
        state.velocityZ * state.velocityZ);
    if (speed < 1.0) speed = 1.0;

    double q = 0.5 * airDensity * speed * speed;

    double thrust = controls.throttle * getMaxThrust();
    double drag = q * wingArea * (dragCoeff + controls.flaps * 0.15);
    double lift = q * wingArea * (liftCoeff + controls.flaps * 2.0) * (1.0 + controls.elevator * 0.2);
    double weight = mass * GlobalConfig::instance().gravity();

    double accelX = (thrust - drag) / mass;
    double accelY = (controls.aileron * 1.5) * q / mass;
    double accelZ = (lift - weight) / mass + (controls.elevator * 10.0);

    outNewState.velocityX = state.velocityX + accelX * deltaTime;
    outNewState.velocityY = state.velocityY + accelY * deltaTime;
    outNewState.velocityZ = state.velocityZ + accelZ * deltaTime;

    outNewState.angularVelocityX = controls.aileron * 15.0 * deltaTime;
    outNewState.angularVelocityY = controls.elevator * 12.0 * deltaTime;
    outNewState.angularVelocityZ = controls.rudder * 10.0 * deltaTime;

    outNewState.thrust = thrust;
    outNewState.drag = drag;
    outNewState.lift = lift;
}