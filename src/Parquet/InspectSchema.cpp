#include "Parquet/SchemaInfo.hpp"
#include "Parquet/InspectSchema.hpp"
#include <parquet/arrow/reader.h>
#include <parquet/arrow/schema.h>
#include <arrow/io/api.h>

arrow::Result<SchemaInfo> inspectParquetSchema(const std::string& file){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file)); // Open the file

    std::unique_ptr<parquet::arrow::FileReader> reader;
    ARROW_ASSIGN_OR_RAISE(reader, parquet::arrow::OpenFile(input, arrow::default_memory_pool()));

    std::shared_ptr<arrow::Schema> schema; // Get the schema with the reader
    ARROW_RETURN_NOT_OK(reader->GetSchema(&schema));

    const auto metadata = reader->parquet_reader()->metadata(); // Read the metadata
    SchemaInfo info;

    info.file = file;
    info.schema = schema->ToString();
    info.numColumns = metadata->num_columns();
    info.numRows = metadata->num_rows();
    info.numRowGroups = metadata->num_row_groups();

    return info;
}