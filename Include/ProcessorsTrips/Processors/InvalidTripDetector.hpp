#pragma once

#include <ProcessorsTrips/TripBatchProcessor.hpp>
#include "ProcessorsTrips/RunStats.hpp"
#include <cstdint>

class InvalidTripDetector final : public TripBatchProcessor{
private:
    std::int64_t validTrips = 0;
    std::int64_t invalidTrips = 0;

    RunStats& invalidTripStats;

public:
    explicit InvalidTripDetector(RunStats& invalidTripStats);

    arrow::Status process(std::int64_t batchSize, const std::string& file) override;

    void finish(const std::string& output) override;
};