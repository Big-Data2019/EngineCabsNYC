#pragma once

#include <arrow/result.h>
#include <vector>
#include <string>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process(
        int64_t batchSize, 
        const std::string& files
    ) = 0;

    virtual void finish(const std::string& output) = 0;
};