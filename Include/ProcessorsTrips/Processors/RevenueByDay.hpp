#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"
#include <unordered_map>

struct RevenueDayStats{
    int64_t files = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;

    double elapsedSeconds = 0.0;
    double totalAmount = 0;

    double quantityPerSecond() const{
        return totalAmount / elapsedSeconds;
    }

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }
};

class RevenueByDay final : public TripBatchProcessor{
private: 
    std::unordered_map<int64_t, double> Accumulator;
    RevenueDayStats& revenueDStats;

public:
    RevenueByDay(RevenueDayStats& revenueDStats);

    arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::string& output) override;
};