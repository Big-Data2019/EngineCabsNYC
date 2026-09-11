#pragma once

#include <arrow/result.h>
#include <string>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process(
        const int64_t& batchSize, 
        const std::vector<std::string>& files
    ) = 0;

    virtual void finish(const std::string& output) = 0;
};