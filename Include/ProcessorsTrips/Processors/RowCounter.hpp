#pragma once

#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/RunStats.hpp"
#include <cstdint>

class RowCounter final : public TripBatchProcessor{
private:
    RunStats& rowCounterStats;

public:
    explicit RowCounter(RunStats& rowCounterStats);

    arrow::Status process(std::int64_t batchSize, const std::string& file) override;

    void finish(const std::string& output) override;
};