#pragma once

#include <arrow/result.h>
#include <string>

arrow::Status inspectSchema(const std::string& file);