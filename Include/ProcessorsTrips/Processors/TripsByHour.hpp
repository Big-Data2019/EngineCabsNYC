#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"

struct TripsHourStats{
    int64_t files = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;

    double elapsedSeconds = 0.0;
    double trips = 0.0;

    double tripsPerSecond() const{
        return trips / elapsedSeconds;
    }

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }
};

class TripsByHour final : public TripBatchProcessor{
private:
    TripsHourStats tripsHStats;

public:
    TripsByHour(TripsHourStats& tripHStats);

    arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::string& output) override;
};