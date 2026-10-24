#pragma once

#include <arrow/result.h>
#include <arrow/api.h>
#include <memory>
#include <string>

arrow::Result<int>
getIdxColumn(const std::shared_ptr<arrow::Schema>& schema, const std::string& nameColumn);