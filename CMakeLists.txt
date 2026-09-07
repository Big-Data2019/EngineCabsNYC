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
find_package(re2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)

# Network library
add_library(Network
    src/Network/CurlGlobal.cpp
    src/Network/CurlEasy.cpp
)

target_include_directories(Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Network
PUBLIC
    CURL::libcurl
)

# Config library
add_library(Config
    src/Config/Config.cpp
    src/Config/DatasetType.cpp
)

target_include_directories(Config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Config
PRIVATE
    re2::re2
    fmt::fmt
PUBLIC
    yaml-cpp::yaml-cpp
)

# Manifest library
add_library(Manifest
    src/Manifest/Json/ParquetDownload.cpp
    src/Manifest/JsonBuilder.cpp
    src/Manifest/JsonGenerator.cpp
)

target_include_directories(Manifest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Manifest
PRIVATE
    spdlog::spdlog
PUBLIC
    Config
    Download
    nlohmann_json::nlohmann_json
)

# Download library
add_library(Download
    src/Download/parquetFiles.cpp
    src/Download/CheckDownload.cpp
)

target_include_directories(Download PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Download
PRIVATE
    Network
    Filesystem
    spdlog::spdlog
PUBLIC
    Config
)

# Filesystem library
add_library(Filesystem
    src/Filesystem/CreateOutputDir.cpp
    src/Filesystem/File.cpp
)

target_include_directories(Filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Filesystem
PRIVATE
    spdlog::spdlog
)

# Parquet library
add_library(Parquet
    src/Parquet/Inspect.cpp
)

target_include_directories(Parquet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Parquet
PRIVATE
    spdlog::spdlog
PUBLIC
    Arrow::arrow_shared
    Parquet::parquet_shared
)

# Cli library
add_library(Cli
    src/Cli/Commands/DownloadCommand.cpp
    src/Cli/Commands/InspectCommand.cpp
    src/Cli/Commands/RunCommand.cpp
    src/Cli/CommandRegister.cpp
)

target_include_directories(Cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Cli
PRIVATE
    Manifest
    Filesystem
    Parquet
    spdlog::spdlog
PUBLIC
    Config
    Processors
    CLI11::CLI11
)

# Processors library
add_library(Processors
    src/ProcessorsTrips/Processors/RawCounter.cpp
    src/Processors/ProcessorRegister.cpp
)

target_include_directories(Processors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(Processors
    PRIVATE
        Arrow::arrow_shared
        Parquet::parquet_shared
        spdlog::spdlog
)

# Main executable
add_executable(EngineCabsNYC src/main.cpp)

target_link_libraries(EngineCabsNYC
PRIVATE
    Network
    Config
    Cli
    Processors
    spdlog::spdlog
    CLI11::CLI11
)

# Test executable
add_executable(EngineCabsNYCTests Tests/TestMain.cpp)

target_link_libraries(EngineCabsNYCTests
PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Benchmark executable
add_executable(EngineCabsNYCBench Benchmarks/BenchMain.cpp)

target_link_libraries(EngineCabsNYCBench
PRIVATE
    benchmark::benchmark
)

# Copy the dataset configuration file to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset_config.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/dataset_config.yaml
    COPYONLY
)