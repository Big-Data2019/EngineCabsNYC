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

# ManifestCore library
add_library(ManifestCore
    src/Manifest/JsonBuilder.cpp
    src/Manifest/JsonGenerator.cpp
)

target_include_directories(ManifestCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(ManifestCore
PRIVATE
    spdlog::spdlog
PUBLIC
    nlohmann_json::nlohmann_json
)

# Manifest library
add_library(Manifest
    src/Manifest/Json/ParquetDownload.cpp
    src/Manifest/Json/RowCounterManifest.cpp
    src/Manifest/Json/InvalidTripManifest.cpp
    src/Manifest/Json/RevenueDayManifest.cpp
    src/Manifest/Json/TripsByHourManifest.cpp
    src/Manifest/Json/TopPickUpManifest.cpp
    src/Manifest/Json/FareDisManifest.cpp
)

target_include_directories(Manifest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Manifest
PRIVATE
    spdlog::spdlog
    Download
    Processors
    ManifestCore
PUBLIC
    Config
)

# Processors library
add_library(Processors
    src/ProcessorsTrips/Processors/RevenueByDay.cpp
    src/ProcessorsTrips/Processors/RowCounter.cpp
    src/ProcessorsTrips/Processors/TripsByHour.cpp
    src/ProcessorsTrips/Processors/InvalidTripDetector.cpp
    src/ProcessorsTrips/Processors/TopPickupZones.cpp
    src/ProcessorsTrips/Processors/FareDistribution.cpp
    src/ProcessorsTrips/ProcessorRegister.cpp
)

target_include_directories(Processors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Processors
PRIVATE
    Parquet::parquet_shared
    spdlog::spdlog
    Utils
    Parquet
    ManifestCore
PUBLIC
    Arrow::arrow_shared
)

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

# Test executable
add_executable(EngineCabsNYCTests Tests/TestMain.cpp)

target_link_libraries(EngineCabsNYCTests
PRIVATE
    GTest::gtest_main
)

# Benchmark executable
add_executable(EngineCabsNYCBench Benchmarks/BenchMain.cpp)

target_link_libraries(EngineCabsNYCBench
PRIVATE
    benchmark::benchmark_main
)

# Copy the dataset configuration file to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset_config.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/dataset_config.yaml
    COPYONLY
)