#include "Parquet/ParquetReader.hpp"
#include <arrow/io/api.h>
#include <utility>

// Function one to retrieve the reader and schema
arrow::Result<ParquetReaderContext> openParquetReader(const std::string& file){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file)); // Open the file

    std::unique_ptr<parquet::arrow::FileReader> reader;
    ARROW_ASSIGN_OR_RAISE(reader, parquet::arrow::OpenFile(input, arrow::default_memory_pool()));

    std::shared_ptr<arrow::Schema> schema; // Get the schema with the reader
    ARROW_RETURN_NOT_OK(reader->GetSchema(&schema));

    return ParquetReaderContext{
        std::move(reader),
        std::move(schema)
    };
}

// Function two to retrieve the schema and the reader with a established batch size
arrow::Result<ParquetReaderContext> openParquetReader(const std::string& file, int64_t batchSize){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file)); // Open the file

    parquet::ArrowReaderProperties readerProperties; // Configure the reader based on properties
    readerProperties.set_batch_size(batchSize);

    parquet::arrow::FileReaderBuilder readerBuilder;
    readerBuilder.properties(readerProperties);

    ARROW_RETURN_NOT_OK(readerBuilder.Open(input));

    std::unique_ptr<parquet::arrow::FileReader> reader;
    ARROW_ASSIGN_OR_RAISE(reader, readerBuilder.Build()); // Construct the reader

    std::shared_ptr<arrow::Schema> schema; // Get the schema with the reader
    ARROW_RETURN_NOT_OK(reader->GetSchema(&schema));

    return ParquetReaderContext{
        std::move(reader),
        std::move(schema)
    };
}