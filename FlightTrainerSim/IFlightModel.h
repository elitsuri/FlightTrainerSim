// File: IFlightModel.h
#ifndef IFLIGHTMODEL_H
#define IFLIGHTMODEL_H

#include <memory>
#include <string>

struct FlightState {
    double velocityX = 0.0;
    double velocityY = 0.0;
    double velocityZ = 0.0;
    double angularVelocityX = 0.0;
    double angularVelocityY = 0.0;
    double angularVelocityZ = 0.0;
    double thrust = 0.0;
    double drag = 0.0;
    double lift = 0.0;
};

struct ControlInputs {
    double elevator = 0.0;
    double aileron = 0.0;
    double rudder = 0.0;
    double throttle = 0.0;
    double flaps = 0.0;
    bool gearDown = true;
};

class IFlightModel {
public:
    virtual ~IFlightModel() = default;
    virtual void computeForces(const FlightState& state, const ControlInputs& controls,
                               double altitude, double deltaTime, FlightState& outNewState) = 0;
    virtual double getMaxThrust() const = 0;
    virtual double getMaxSpeed() const = 0;
    virtual double getStallSpeed() const = 0;
    virtual double getFuelConsumptionRate() const = 0;
    virtual std::string getModelName() const = 0;
};

class TrainerFlightModel : public IFlightModel {
public:
    void computeForces(const FlightState&, const ControlInputs&, double, double, FlightState&) override;
    double getMaxThrust() const override { return 12000.0; }
    double getMaxSpeed() const override { return 250.0; }
    double getStallSpeed() const override { return 55.0; }
    double getFuelConsumptionRate() const override { return 15.0; }
    std::string getModelName() const override { return "T-38 Trainer"; }
};

class JetFlightModel : public IFlightModel {
public:
    void computeForces(const FlightState&, const ControlInputs&, double, double, FlightState&) override;
    double getMaxThrust() const override { return 128000.0; }
    double getMaxSpeed() const override { return 1200.0; }
    double getStallSpeed() const override { return 120.0; }
    double getFuelConsumptionRate() const override { return 120.0; }
    std::string getModelName() const override { return "F-16 Fighting Falcon"; }
};

class CargoFlightModel : public IFlightModel {
public:
    void computeForces(const FlightState&, const ControlInputs&, double, double, FlightState&) override;
    double getMaxThrust() const override { return 180000.0; }
    double getMaxSpeed() const override { return 470.0; }
    double getStallSpeed() const override { return 105.0; }
    double getFuelConsumptionRate() const override { return 200.0; }
    std::string getModelName() const override { return "C-130 Hercules"; }
};

#endif
