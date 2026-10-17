# Parquet library
add_library(Parquet
    InspectSchema.cpp
    IdxColumnSchema.cpp
    ParquetReader.cpp
    PrintSchemaInfo.cpp
)

target_include_directories(Parquet PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Parquet
PRIVATE
    spdlog::spdlog
    Parquet::parquet_shared
PUBLIC
    Arrow::arrow_shared
)
