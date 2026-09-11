#pragma once

#include <vector>
#include <string>

struct ProcessorData{
    int64_t batchSize;
    std::vector<std::string> processors;
    std::vector<std::string> filesToProcess;
    std::string output;
};