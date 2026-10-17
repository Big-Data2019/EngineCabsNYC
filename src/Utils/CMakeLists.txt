# Utils library
add_library(Utils
    ExpanderFiles.cpp
)

target_include_directories(Utils PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Utils
PRIVATE
    spdlog::spdlog
)

