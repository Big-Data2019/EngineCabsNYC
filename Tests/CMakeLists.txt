# Add source files
find_package(GTest CONFIG REQUIRED)

# Test executable
add_executable(EngineCabsNYCTests TestMain.cpp)

target_link_libraries(EngineCabsNYCTests
PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(EngineCabsNYCTests)