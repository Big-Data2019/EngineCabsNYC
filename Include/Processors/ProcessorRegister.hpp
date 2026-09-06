#pragma once

#include "Include/Processors/BatchProcessor.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class ProcessorRegister final{
private:
    std::unordered_map<std::string, std::unique_ptr<TripBatchProcessor>> processors;

public:
    void registerProcessors();
};