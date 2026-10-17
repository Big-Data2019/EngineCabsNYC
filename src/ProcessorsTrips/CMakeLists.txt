# Processors library
add_library(Processors
    Processors/RevenueByDay.cpp
    Processors/RowCounter.cpp
    Processors/TripsByHour.cpp
    Processors/InvalidTripDetector.cpp
    Processors/TopPickupZones.cpp
    Processors/FareDistribution.cpp
    ProcessorRegister.cpp
)

target_include_directories(Processors PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Processors
PRIVATE
    Parquet::parquet_shared
    spdlog::spdlog
    Utils
    Parquet
    ManifestCore
PUBLIC
    Arrow::arrow_shared
)
