#pragma once

#include <arrow/result.h>
#include <string>

arrow::Result<int> getIdxColumn(const std::string& file, const std::string& nameColumn);