#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"
#include <map>

struct TripsHourStats{
    int8_t files = 0;
    int64_t numRows = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;

    double elapsedSeconds = 0.0;
    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }

    double rowsPerSecond() const{
        return numRows / elapsedSeconds;
    }
};

class TripsByHour final : public TripBatchProcessor{
private:
    std::map<int64_t, int64_t> tripsByHour;
    TripsHourStats tripsHStats;

public:
    TripsByHour(TripsHourStats& tripHStats);

    arrow::Status process(
        int64_t batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::string& output) override;
};