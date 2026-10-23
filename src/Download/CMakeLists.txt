# Download library
add_library(Download
    ParquetFiles.cpp
    VerifyDownload.cpp
)

target_include_directories(Download PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Download
PRIVATE
    Network
    Filesystem
    spdlog::spdlog
PUBLIC
    Config
)