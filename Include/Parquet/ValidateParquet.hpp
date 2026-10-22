#pragma once

#include "Parquet/ValidateInfo.hpp"
#include "arrow/result.h"
#include <vector>
#include <string>

arrow::Result<ValidateInfo> validateParquetSchema(const std::vector<std::string>& file);