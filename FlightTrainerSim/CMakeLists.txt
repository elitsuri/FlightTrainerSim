cmake_minimum_required(VERSION 3.16)
project(FlightTrainerSim VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

set(SOURCES
    main.cpp
    GlobalConfig.h
    IFlightModel.h IFlightModel.cpp
    Aircraft.h Aircraft.cpp
    Environment.h Environment.cpp
    TrainingScenario.h TrainingScenario.cpp
    AircraftFactory.h
    FlightMetrics.h FlightMetrics.cpp
    AudioSystem.h AudioSystem.cpp
    SimulationEngine.h SimulationEngine.cpp
    Cockpit3DView.h Cockpit3DView.cpp
    Outside3DView.h Outside3DView.cpp
    FlightControlPanel.h FlightControlPanel.cpp
    DebriefWindow.h DebriefWindow.cpp
    MainWindow.h MainWindow.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)