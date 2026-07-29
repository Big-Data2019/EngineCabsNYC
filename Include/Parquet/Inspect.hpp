#pragma once
#include <arrow/api.h>
#include <arrow/io/file.h>
#include <parquet/arrow/reader.h>
#include <string>

arrow::Status inspectSchema(const std::string& file);