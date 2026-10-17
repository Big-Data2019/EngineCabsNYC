# Filesystem library
add_library(Filesystem
    CreateOutputDir.cpp
    File.cpp
)

target_include_directories(Filesystem PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Filesystem
PRIVATE
    spdlog::spdlog
)