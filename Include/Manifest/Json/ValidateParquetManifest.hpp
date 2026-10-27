#pragma once

#include "Parquet/InvalidInfo.hpp"
#include <string>

void generateValidationManifest(const InvalidInfo& info, const std::string& output);