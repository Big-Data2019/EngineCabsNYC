#pragma once

#include "Parquet/SchemaInfo.hpp"
#include <arrow/result.h>
#include <string>

arrow::Result<SchemaInfo> inspectParquetSchema(const std::string& file);