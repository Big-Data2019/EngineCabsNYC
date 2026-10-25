#pragma once

#include <arrow/status.h>
#include <vector>
#include <string>
#include <cstdint>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process(
        std::int64_t batchSize,
        const std::string& file
    ) = 0;

    virtual void finish(const std::string& output) = 0;
};