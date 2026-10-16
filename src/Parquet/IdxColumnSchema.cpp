#include "Parquet/IdxColumnSchema.hpp"
#include <parquet/arrow/reader.h>
#include <parquet/arrow/schema.h>
#include <arrow/io/api.h>
#include <memory>

arrow::Result<int> getIdxColumn(const std::string& file, const std::string& nameColumn){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file));

    std::unique_ptr<parquet::arrow::FileReader> arrowReader;
    ARROW_ASSIGN_OR_RAISE(arrowReader,
        parquet::arrow::OpenFile(input, arrow::default_memory_pool())
    );

    std::shared_ptr<arrow::Schema> schema;
    ARROW_RETURN_NOT_OK(arrowReader->GetSchema(&schema));

    int idx = schema->GetFieldIndex(nameColumn);
    if(idx == -1) return arrow::Status::Invalid("The required column was not found");

    return idx;
}