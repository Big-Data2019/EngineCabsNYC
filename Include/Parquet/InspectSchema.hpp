#pragma once

#include "Parquet/ParquetSchemaInfo.hpp"
#include <arrow/result.h>
#include <string>

arrow::Result<SchemaInfo> inspectParquetSchema(const std::string& file);