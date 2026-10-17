#include "Parquet/PrintSchemaInfo.hpp"
#include <spdlog/spdlog.h>

void printSchemaInfo(const SchemaInfo& info){
    spdlog::info("File: {}", info.file);
    spdlog::info("Num Columns: {}", info.numColumns);
    spdlog::info("Num Rows: {}", info.numRows);
    spdlog::info("Num Row Groups: {}", info.numRowGroups);
    spdlog::info("Schema: \n{}", info.schema);
}