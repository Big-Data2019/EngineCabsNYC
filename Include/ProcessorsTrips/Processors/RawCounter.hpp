#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"

struct RawStats{
    int64_t files = 0;
    int64_t numRows = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    
    double elapsedSeconds = 0.0;
    double rowsPerSecond = 0.0;
    double megabytesPerSecond = 0.0;
};

class RawCounter final : public TripBatchProcessor{
private:
    RawStats& stats;

public:
    RawCounter(RawStats& stats);

    arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::filesystem::path& output) override;
};