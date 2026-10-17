# Cli library
add_library(Cli
    Commands/DownloadCommand.cpp
    Commands/InspectCommand.cpp
    Commands/RunCommand.cpp
    CommandRegister.cpp
)

target_include_directories(Cli PUBLIC ${PROJECT_SOURCE_DIR}/Include)

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