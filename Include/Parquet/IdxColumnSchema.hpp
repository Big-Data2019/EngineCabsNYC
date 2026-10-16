#pragma once

#include <arrow/result.h>
#include <string>

arrow::Result<int> getSchemaIdx(const std::string& file, const std::string& nameColumn);