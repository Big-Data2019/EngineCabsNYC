# Add source files
find_package(nlohmann_json CONFIG REQUIRED)

# ManifestCore library
add_library(ManifestCore
    JsonBuilder.cpp
    JsonGenerator.cpp
)

target_include_directories(ManifestCore PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(ManifestCore
PRIVATE
    spdlog::spdlog
PUBLIC
    nlohmann_json::nlohmann_json
)

# Manifest library
add_library(Manifest
    Json/ParquetDownload.cpp
    Json/ValidateParquetManifest.cpp
    Json/RowCounterManifest.cpp
    Json/InvalidTripManifest.cpp
    Json/RevenueDayManifest.cpp
    Json/TripsByHourManifest.cpp
    Json/TopPickUpManifest.cpp
    Json/FareDisManifest.cpp
)

target_include_directories(Manifest PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Manifest
PRIVATE
    spdlog::spdlog
    Download
    Processors
    ManifestCore
PUBLIC
    Config
)
