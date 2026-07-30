cmake_minimum_required(VERSION 3.16)
project(EngineCabsNYC)

set(CMAKE_CXX_STANDARD 20)

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

# Add main executable
add_executable(EngineCabsNYC 
    src/main.cpp
    src/Network/CurlGlobal.cpp
    src/Network/CurlEasy.cpp
    src/Config/ConfigYaml.cpp
    src/Config/ConfigGlobal.cpp
    src/Config/DatasetType.cpp
    src/Filesystem/EnsureDirectory.cpp
    src/Download/parquetFiles.cpp
    src/Parquet/Inspect.cpp
    src/Manifest/BuildJson.cpp
    src/Manifest/Generatejson.cpp
    src/Filesystem/File.cpp
    src/Download/CheckDownload.cpp
    src/Cli/Commands/DownloadCommand.cpp
    src/Cli/Commands/InspectCommand.cpp
    src/Cli/CommandHandler.cpp
)

target_link_libraries(EngineCabsNYC PRIVATE 
    yaml-cpp 
    spdlog::spdlog
    fmt::fmt
    CLI11::CLI11
    Arrow::arrow_shared
    Parquet::parquet_shared
    re2::re2
    nlohmann_json::nlohmann_json
    CURL::libcurl
)

add_executable(EngineCabsNYC_tests tests/test_main.cpp)

target_link_libraries(EngineCabsNYC_tests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

add_executable(EngineCabsNYC_bench benchmarks/bench_main.cpp)

target_link_libraries(EngineCabsNYC_bench PRIVATE 
    benchmark::benchmark 
)

# Copy the dataset configuration file to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset_config.yaml 
    ${CMAKE_CURRENT_BINARY_DIR}/dataset_config.yaml 
    COPYONLY
)

target_include_directories(EngineCabsNYC PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)