#pragma once

#include <vector>
#include <string>
#include <cstdint>

struct ProcessorData{
    int64_t batchSize;
    std::vector<std::string> processors;
    std::vector<std::string> filesToProcess;
    std::string output;
};