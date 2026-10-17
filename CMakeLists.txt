cmake_minimum_required(VERSION 3.16)
project(EngineCabsNYC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    add_compile_definitions(NOMINMAX)
endif()

# Add source files
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(Arrow CONFIG REQUIRED)
find_package(Parquet CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)

add_subdirectory(src)

# Main executable
add_executable(EngineCabsNYC src/main.cpp)

target_link_libraries(EngineCabsNYC
PRIVATE
    Network
    Config
    Cli
    spdlog::spdlog
    CLI11::CLI11
)

# Tests configuration and enablement
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

# Benchmarks configuration and enablement
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
if(BUILD_BENCHMARKS)
    add_subdirectory(Benchmarks)
endif()

# Copy the dataset configuration file to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset_config.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/dataset_config.yaml
    COPYONLY
)