// File: Aircraft.h
#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "IFlightModel.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

struct Position3D {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Orientation {
    double heading = 0.0;
    double pitch = 0.0;
    double bank = 0.0;
};

class Aircraft {
public:
    explicit Aircraft(std::unique_ptr<IFlightModel> flightModel);
    void update(double deltaTime);
    void reset();
    
    const Position3D& position() const { return m_position; }
    const Orientation& orientation() const { return m_orientation; }
    double altitude() const { return m_position.z; }
    double speed() const;
    double verticalSpeed() const { return m_flightState.velocityZ; }
    double heading() const { return m_orientation.heading; }
    double pitch() const { return m_orientation.pitch; }
    double bank() const { return m_orientation.bank; }
    double fuel() const { return m_fuel; }
    double thrust() const { return m_flightState.thrust; }
    bool isStalled() const;
    bool isOnGround() const { return m_position.z <= 0.1; }
    
    const FlightState& flightState() const { return m_flightState; }
    const ControlInputs& controls() const { return m_controls; }
    const IFlightModel* flightModel() const { return m_flightModel.get(); }
    
    void setPosition(const Position3D& pos) { m_position = pos; }
    void setOrientation(const Orientation& orient) { m_orientation = orient; }
    void setControls(const ControlInputs& controls) { m_controls = controls; }
    void setFuel(double fuel) { m_fuel = fuel; }
    
    void setElevator(double value) { m_controls.elevator = std::clamp(value, -1.0, 1.0); }
    void setAileron(double value) { m_controls.aileron = std::clamp(value, -1.0, 1.0); }
    void setRudder(double value) { m_controls.rudder = std::clamp(value, -1.0, 1.0); }
    void setThrottle(double value) { m_controls.throttle = std::clamp(value, 0.0, 1.0); }
    void setFlaps(double value) { m_controls.flaps = std::clamp(value, 0.0, 1.0); }
    void setGear(bool down) { m_controls.gearDown = down; }
    
    const std::vector<Position3D>& flightPath() const { return m_flightPath; }
    void recordPosition();
    
private:
    std::unique_ptr<IFlightModel> m_flightModel;
    Position3D m_position;
    Orientation m_orientation;
    FlightState m_flightState;
    ControlInputs m_controls;
    double m_fuel;
    std::vector<Position3D> m_flightPath;
    double m_pathRecordTimer;
    
    void updatePhysics(double deltaTime);
    void updateOrientation(double deltaTime);
    void updateFuel(double deltaTime);
    void enforceConstraints();
};

#endif
