#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"

class RawCounter final : public TripBatchProcessor{
private:
    int64_t numRows = 0;

public:
    RawCounter(int64_t& numRows_);

    arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::filesystem::path& output) override;
};