// File: IFlightModel.cpp
#include "IFlightModel.h"
#include "GlobalConfig.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {
    double airDensity(double altitude) {
        return 1.225 * std::exp(-altitude / 10000.0);
    }
    double computeDrag(double velocity, double altitude, double area, double coeff) {
        double rho = airDensity(altitude);
        return 0.5 * rho * velocity * velocity * area * coeff;
    }
    double computeLift(double velocity, double altitude, double area, double coeff, double aoa) {
        double rho = airDensity(altitude);
        double effectiveCoeff = coeff * (1.0 + 0.1 * std::sin(aoa * M_PI / 180.0));
        return 0.5 * rho * velocity * velocity * area * effectiveCoeff;
    }
}

void TrainerFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
                                       double altitude, double deltaTime, FlightState& outNewState) {
    double speed = std::sqrt(state.velocityX*state.velocityX + state.velocityY*state.velocityY + state.velocityZ*state.velocityZ);
    double thrust = controls.throttle * getMaxThrust();
    double drag = computeDrag(speed, altitude, 15.8, 0.024);
    double angleOfAttack = controls.elevator * 15.0;
    double lift = computeLift(speed, altitude, 15.8, 0.45, angleOfAttack);
    lift *= (1.0 + controls.flaps * 0.3);
    drag *= (1.0 + controls.flaps * 0.4);
    if (controls.gearDown) drag *= 1.15;
    double accelX = (thrust - drag) / 5300.0;
    double accelY = controls.aileron * 20.0;
    double accelZ = (lift - 5300.0 * GlobalConfig::instance().gravity()) / 5300.0;
    outNewState.velocityX = state.velocityX + accelX * deltaTime;
    outNewState.velocityY = state.velocityY + accelY * deltaTime;
    outNewState.velocityZ = state.velocityZ + accelZ * deltaTime;
    outNewState.angularVelocityX = controls.aileron * 90.0 * deltaTime;
    outNewState.angularVelocityY = controls.elevator * 45.0 * deltaTime;
    outNewState.angularVelocityZ = controls.rudder * 30.0 * deltaTime;
    outNewState.thrust = thrust;
    outNewState.drag = drag;
    outNewState.lift = lift;
}

void JetFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
                                   double altitude, double deltaTime, FlightState& outNewState) {
    double speed = std::sqrt(state.velocityX*state.velocityX + state.velocityY*state.velocityY + state.velocityZ*state.velocityZ);
    double thrustMult = (controls.throttle > 0.95) ? 1.5 : 1.0;
    double thrust = controls.throttle * getMaxThrust() * thrustMult;
    double drag = computeDrag(speed, altitude, 27.87, 0.021);
    double angleOfAttack = controls.elevator * 25.0;
    double lift = computeLift(speed, altitude, 27.87, 0.52, angleOfAttack);
    lift *= (1.0 + controls.flaps * 0.25);
    drag *= (1.0 + controls.flaps * 0.35);
    if (controls.gearDown) drag *= 1.12;
    double accelX = (thrust - drag) / 12000.0;
    double accelY = controls.aileron * 35.0;
    double accelZ = (lift - 12000.0 * GlobalConfig::instance().gravity()) / 12000.0;
    outNewState.velocityX = state.velocityX + accelX * deltaTime;
    outNewState.velocityY = state.velocityY + accelY * deltaTime;
    outNewState.velocityZ = state.velocityZ + accelZ * deltaTime;
    outNewState.angularVelocityX = controls.aileron * 120.0 * deltaTime;
    outNewState.angularVelocityY = controls.elevator * 60.0 * deltaTime;
    outNewState.angularVelocityZ = controls.rudder * 45.0 * deltaTime;
    outNewState.thrust = thrust;
    outNewState.drag = drag;
    outNewState.lift = lift;
}

void CargoFlightModel::computeForces(const FlightState& state, const ControlInputs& controls,
                                     double altitude, double deltaTime, FlightState& outNewState) {
    double speed = std::sqrt(state.velocityX*state.velocityX + state.velocityY*state.velocityY + state.velocityZ*state.velocityZ);
    double thrust = controls.throttle * getMaxThrust();
    double drag = computeDrag(speed, altitude, 162.12, 0.031);
    double angleOfAttack = controls.elevator * 12.0;
    double lift = computeLift(speed, altitude, 162.12, 0.38, angleOfAttack);
    lift *= (1.0 + controls.flaps * 0.4);
    drag *= (1.0 + controls.flaps * 0.5);
    if (controls.gearDown) drag *= 1.2;
    double accelX = (thrust - drag) / 70000.0;
    double accelY = controls.aileron * 12.0;
    double accelZ = (lift - 70000.0 * GlobalConfig::instance().gravity()) / 70000.0;
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
