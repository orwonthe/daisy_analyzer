cmake_minimum_required (VERSION 3.11)
project(daisy_analyzer)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES
        src/DaisyAnalyzer.cpp
        src/DaisyAnalyzer.h
        src/DaisyAnalyzerResults.cpp
        src/DaisyAnalyzerResults.h
        src/DaisyAnalyzerSettings.cpp
        src/DaisyAnalyzerSettings.h
        src/DaisySimulationDataGenerator.cpp
        src/DaisySimulationDataGenerator.h
)

add_analyzer_plugin(daisy_analyzer SOURCES ${SOURCES})
