// File: AircraftFactory.h
#ifndef AIRCRAFTFACTORY_H
#define AIRCRAFTFACTORY_H

#include "Aircraft.h"
#include "IFlightModel.h"
#include <memory>

enum class AircraftType { Trainer, Jet, Cargo };

class AircraftFactory {
public:
    static std::unique_ptr<Aircraft> createAircraft(AircraftType type) {
        std::unique_ptr<IFlightModel> model;
        switch (type) {
            case AircraftType::Trainer: model = std::make_unique<TrainerFlightModel>(); break;
            case AircraftType::Jet: model = std::make_unique<JetFlightModel>(); break;
            case AircraftType::Cargo: model = std::make_unique<CargoFlightModel>(); break;
            default: model = std::make_unique<TrainerFlightModel>();
        }
        return std::make_unique<Aircraft>(std::move(model));
    }
    static std::string getAircraftTypeName(AircraftType type) {
        switch (type) {
            case AircraftType::Trainer: return "Trainer";
            case AircraftType::Jet: return "Jet Fighter";
            case AircraftType::Cargo: return "Cargo Aircraft";
            default: return "Unknown";
        }
    }
};

#endif
