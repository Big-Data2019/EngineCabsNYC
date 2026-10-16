#include "Parquet/ParquetReader.hpp"
#include <arrow/io/api.h>
#include <utility>

arrow::Result<ParquetReaderContext> openParquetReader(const std::string& file, const int64_t& batchSize){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file)); // Open the file

    parquet::ArrowReaderProperties readerProperties; // Configure the reader based on properties
    readerProperties.set_batch_size(batchSize);

    parquet::arrow::FileReaderBuilder readerBuilder;
    readerBuilder.properties(readerProperties);

    ARROW_RETURN_NOT_OK(readerBuilder.Open(input));

    std::unique_ptr<parquet::arrow::FileReader> arrowReader;
    ARROW_ASSIGN_OR_RAISE(arrowReader, readerBuilder.Build()); // Construct the reader

    std::shared_ptr<arrow::Schema> schema; // Get the schema with the reader
    ARROW_RETURN_NOT_OK(arrowReader->GetSchema(&schema));

    return ParquetReaderContext{
        std::move(arrowReader),
        std::move(schema)
    };
}