#pragma once

#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/RunStats.hpp"
#include <cstdint>
#include <map>

class RevenueByDay final : public TripBatchProcessor{
private: 
    std::map<std::int64_t, double> Accumulator;
    RunStats& revenueDayStats;

public:
    explicit RevenueByDay(RunStats& revenueDayStats);

    arrow::Status process(std::int64_t batchSize, const std::string& file) override;

    void finish(const std::string& output) override;
};