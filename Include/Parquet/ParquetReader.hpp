#pragma once

#include <parquet/arrow/reader.h>
#include <arrow/api.h>
#include <memory>
#include <string>

struct ParquetReaderContext{
    std::unique_ptr<parquet::arrow::FileReader> reader;
    std::shared_ptr<arrow::Schema> schema;
};

arrow::Result<ParquetReaderContext> openParquetReader(
    const std::string& file,
    const int64_t& batchSize
);