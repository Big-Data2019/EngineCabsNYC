#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"

struct RevenueDayStats{
    int64_t files = 0;
    int64_t numRows = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;

    double elapsedSeconds = 0.0;
    double rowsPerSecond() const{
        return numRows / elapsedSeconds;
    }

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }
};

class RevenueByDay final : public TripBatchProcessor{
private:
    RevenueDayStats& revenueDStats;

public:
    RevenueByDay(RevenueDayStats& revenueDStats);

    virtual arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    virtual void finish(const std::string& output) override;
};