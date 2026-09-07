#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"
#include "ProcessorsTrips/ProcessorData.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class ProcessorRegister final{
private:
    std::unordered_map<std::string, std::unique_ptr<TripBatchProcessor>> processors;

public:
    void registerProcessors();

    void searchProcessor(const ProcessorData& data);
};