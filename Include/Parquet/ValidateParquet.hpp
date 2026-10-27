#pragma once

#include "arrow/result.h"
#include "Parquet/InvalidInfo.hpp"
#include <string>

arrow::Result<InvalidInfo> validateParquetData(const std::string& file);