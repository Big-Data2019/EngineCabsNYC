# Add source files
find_package(yaml-cpp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Config library
add_library(Config
    Config.cpp
    DatasetType.cpp
)

target_include_directories(Config PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Config
PRIVATE
    fmt::fmt
PUBLIC
    yaml-cpp::yaml-cpp
)