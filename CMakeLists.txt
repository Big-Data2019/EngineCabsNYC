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


# Add main executable
add_executable(EngineCabsNYC src/main.cpp)

target_link_libraries(EngineCabsNYC PRIVATE 
    yaml-cpp 
    spdlog::spdlog
    fmt::fmt
    CLI11::CLI11
    Arrow::arrow_shared
    Parquet::parquet_shared
)

# Add test executable
add_executable(EngineCabsNYC_tests tests/test_main.cpp)

target_link_libraries(EngineCabsNYC_tests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

# Add benchmark executable
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