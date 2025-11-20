# File: CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(FlightTrainerSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Multimedia)

set(PROJECT_SOURCES
    main.cpp
    GlobalConfig.cpp
    TrainerFlightModel.cpp
    JetFlightModel.cpp
    CargoFlightModel.cpp
    Aircraft.cpp
    Environment.cpp
    ScenarioState.cpp
    TrainingScenario.cpp
    FlightMetrics.cpp
    DebriefReport.cpp
    AircraftFactory.cpp
    SimulationEngine.cpp
    Cockpit3DView.cpp
    Outside3DView.cpp
    FlightControlPanel.cpp
    DebriefWindow.cpp
    MainWindow.cpp
    AudioManager.cpp
)

set(PROJECT_HEADERS
    GlobalConfig.h
    IFlightModel.h
    TrainerFlightModel.h
    JetFlightModel.h
    CargoFlightModel.h
    Aircraft.h
    Environment.h
    ScenarioState.h
    TrainingScenario.h
    FlightMetrics.h
    DebriefReport.h
    AircraftFactory.h
    SimulationEngine.h
    Cockpit3DView.h
    Outside3DView.h
    FlightControlPanel.h
    DebriefWindow.h
    MainWindow.h
    AudioManager.h
)

add_executable(FlightTrainerSim
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_link_libraries(FlightTrainerSim PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Multimedia
)

set_target_properties(FlightTrainerSim PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
