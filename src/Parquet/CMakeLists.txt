# Parquet library
add_library(Parquet
    IdxColumnSchema.cpp
    ParquetReader.cpp
    ValidateParquet.cpp
)

target_include_directories(Parquet PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Parquet
PRIVATE
    spdlog::spdlog
PUBLIC
    Arrow::arrow_shared
    Parquet::parquet_shared
)
