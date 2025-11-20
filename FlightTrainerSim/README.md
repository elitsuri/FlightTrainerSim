# FlightTrainerSim - Professional Flight Training Simulator

## Overview
Professional flight training simulator with 3D graphics, realistic physics, and comprehensive training scenarios.

## Features
- 3 Aircraft types: T-38 Trainer, F-16 Fighter, C-130 Cargo
- 4 Training scenarios: Takeoff, Pattern, IFR, Emergency
- Advanced 3D HUD cockpit display
- External 3D view with flight path visualization
- Real-time performance analysis

## Build Requirements
- CMake 3.16+ or qmake
- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Qt 6.2+ (Core, GUI, Widgets modules)

## Building

### Linux/Mac (CMake)
```bash
./build.sh cmake
cd build/bin
./FlightTrainerSim
```

### Linux/Mac (qmake)
```bash
./build.sh qmake
./FlightTrainerSim
```

### Windows (CMake)
```cmd
build.bat cmake
cd build\bin
FlightTrainerSim.exe
```

### Windows (qmake)
```cmd
build.bat qmake
cd release
FlightTrainerSim.exe
```

## Usage
1. Select aircraft type and scenario from toolbar
2. Click "Start" to begin simulation
3. Use control panel to adjust flight controls
4. Complete scenario objectives
5. Review performance in debrief window

## Controls
- Elevator: Pitch control (-100% to +100%)
- Aileron: Roll control (-100% to +100%)
- Rudder: Yaw control (-100% to +100%)
- Throttle: Engine power (0-100%)
- Flaps: High-lift devices (0-100%)
- Landing Gear: Extend/retract

## Design Patterns
- Strategy: IFlightModel with 3 implementations
- State: ScenarioState machine
- Factory: AircraftFactory
- Observer: Qt signals/slots
- Singleton: GlobalConfig

## License
Defense Aviation Systems - All rights reserved

## Version
1.0.0
