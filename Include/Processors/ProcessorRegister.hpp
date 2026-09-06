#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <arrow/result.h>

class ProcessorRegister final{
private:
    std::unordered_map <   
        std::string, 
        std::function <
            arrow::Status(
                const int64_t& batchSize,
                const std::vector<std::string>& files
        ) >
    > processors;
};