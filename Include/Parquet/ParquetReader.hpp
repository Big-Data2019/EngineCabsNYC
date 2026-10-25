#pragma once

#include <parquet/arrow/reader.h>
#include <parquet/arrow/schema.h>
#include <arrow/result.h>
#include <memory>
#include <string>

struct ParquetReaderContext{
    std::unique_ptr<parquet::arrow::FileReader> reader;
    std::shared_ptr<arrow::Schema> schema;
};

arrow::Result<ParquetReaderContext> openParquetReader(const std::string& file);

arrow::Result<ParquetReaderContext>
openParquetReader(const std::string& file, std::int64_t batchSize);