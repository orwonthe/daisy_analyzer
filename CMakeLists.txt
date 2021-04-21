cmake_minimum_required (VERSION 3.11)
project(daisy_analyzer)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES
        src/AnalyzerChannelDataHolder.cpp
        src/AnalyzerChannelDataHolder.h
        src/BitGenerator.cpp
        src/BitGenerator.h
        src/ChannelizedAnalyzerSettings.cpp
        src/ChannelizedAnalyzerSettings.h
        src/Channelizer.cpp
        src/Channelizer.h
        src/ChannelizerManager.cpp
        src/ChannelizerManager.h
        src/ChannelizerOwner.cpp
        src/ChannelizerOwner.h
        src/Clocker.cpp
        src/Clocker.h
        src/ClockingChannelizer.cpp
        src/ClockingChannelizer.h
        src/ClockingChannelizerManager.cpp
        src/ClockingChannelizerManager.h
        src/DaisyAnalyzer.cpp
        src/DaisyAnalyzer.h
        src/DaisyAnalyzerResults.cpp
        src/DaisyAnalyzerResults.h
        src/DaisyAnalyzerSettings.cpp
        src/DaisyAnalyzerSettings.h
        src/DaisyChannelizerManager.cpp
        src/DaisyChannelizerManager.h
        src/DaisySimulationDataGenerator.cpp
        src/DaisySimulationDataGenerator.h
        src/DataChannelizer.cpp
        src/DataChannelizer.h
        src/DaisyChannelizerManager.cpp
        src/DaisyChannelizerManager.h
        src/MasterChannelizerManager.cpp
        src/MasterChannelizerManager.h
        src/SignalGrabber.cpp
        src/SignalGrabber.h
)

add_analyzer_plugin(daisy_analyzer SOURCES ${SOURCES})
