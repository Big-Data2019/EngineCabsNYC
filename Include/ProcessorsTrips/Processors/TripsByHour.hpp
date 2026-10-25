#pragma once

#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/RunStats.hpp"
#include <cstdint>
#include <map>

class TripsByHour final : public TripBatchProcessor{
private:
    std::map<int64_t, int64_t> tripsByHour;
    RunStats tripsHourStats;

public:
    explicit TripsByHour(RunStats& tripsHourStats);

    arrow::Status process(std::int64_t batchSize, const std::string& file) override;

    void finish(const std::string& output) override;
};